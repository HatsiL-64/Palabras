//La segunda pantalla es la seleccion de dificultad, tambien se asigna las palabras aleatorias
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <ctype.h>
#include <string.h> 
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define XMAX 1280
#define YMAX 720


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

char* mstrdup(char* s){
  char* copy = malloc(strlen(s) + 1);
  if(copy != NULL)
    strcpy(copy, s);
  return copy;
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

void pantalla2(ALLEGRO_FONT *font, ALLEGRO_BITMAP *fondo, ALLEGRO_BITMAP *extra, int x, int y){
  ALLEGRO_COLOR negro = al_map_rgb(0, 0, 0);
  ALLEGRO_COLOR blanco = al_map_rgb(255, 255, 255);

  al_draw_bitmap(fondo, 0, 0, 0);
  al_draw_filled_rounded_rectangle(557.2, 103, 722.8, 206, 10, 10, negro);
  al_draw_text(font, blanco, 640, 129.5, ALLEGRO_ALIGN_CENTER, "Facil");
  al_draw_filled_rounded_rectangle(557.2, 308, 722.8, 411, 10, 10, negro);
  al_draw_text(font, blanco, 640, 334.5, ALLEGRO_ALIGN_CENTER, "Medio");
  al_draw_filled_rounded_rectangle(557.2, 514, 722.8, 617, 10, 10, negro);
  al_draw_text(font, blanco, 640, 540.5, ALLEGRO_ALIGN_CENTER, "Dificil");
  al_draw_bitmap(extra, x, y, 0);
  al_flip_display();
}

char **obtienePalabras(int opc, int *palabrasn){
  if(opc == 1)
    *palabrasn = 5;
  if(opc == 2)
    *palabrasn = 8;
  if(opc == 3)
    *palabrasn = 11;

  char temp, *stemp;
  int nLineas = 0, *indice;
  char **palabras = malloc(sizeof(char*) * (*palabrasn));
  FILE *archPalabras = NULL;
  archPalabras = fopen("src/palabras.txt", "r");
  if(archPalabras == NULL){
    return NULL;
  }
  
  while ((temp = fgetc(archPalabras)) != EOF) { //Cuenta la cantidad de lineas 
    if(temp == '\n')
      nLineas++;
  }

  indice = malloc(sizeof(int) * nLineas);
  for(int i = 0; i < nLineas; i++){
    indice[i] = i;
  }

  desorden(indice, nLineas);
  sort(indice, *palabrasn);

  int k = 0;
  rewind(archPalabras);
  for(int i = 0; i < nLineas; i++){
    if(k < *palabrasn){
      if(i == indice[k]){
        palabras[k] = scanif(archPalabras);
        k++;
      }
      else {
        stemp = scanif(archPalabras);
      }
    }
  }

  fclose(archPalabras);
  free(indice);
  free(stemp);
  return palabras;
}

int main()
{
  if(!al_init()){
    return -1;
  }
  if(!al_init_primitives_addon()){
    return -1;
  }
  if(!al_init_image_addon()){
    return -1;
  }
  if(!al_install_keyboard()){
    return -1;
  }
  if(!al_init_font_addon()){
    return -1;
  }
  if(!al_init_ttf_addon()){
    return -1;
  }

  ALLEGRO_DISPLAY *disp;
  ALLEGRO_EVENT_QUEUE *eventos;
  ALLEGRO_EVENT evento;
  ALLEGRO_FONT *fuenteTexto, *fuentePalabras, *fuenteRespuesta;
  ALLEGRO_BITMAP *portada, *assets, *background;
  
  int pantalla = 2;

  srand(time(NULL));

  disp = al_create_display(XMAX, YMAX);
  al_set_window_title(disp, "Palabras");

  portada = al_load_bitmap("statics/img/PORTADA.png");

  fuenteTexto = al_load_ttf_font("statics/fonts/Swansea-q3pd.ttf", 30, 0);
  fuenteRespuesta = al_load_ttf_font("statics/fonts/SwanseaBold-D0ox.ttf", 50, 0);
  fuentePalabras = al_load_ttf_font("statics/fonts/SwanseaBoldItalic-p3Dv.ttf", 100, 0);

  eventos = al_create_event_queue();
  al_register_event_source(eventos, al_get_display_event_source(disp));
  al_register_event_source(eventos, al_get_keyboard_event_source());
  
  // A partir de aqui copiar al codigo original
  background = al_load_bitmap("statics/img/bg.png");
  assets = al_load_bitmap("statics/img/Flecha.png");
  int x = 850, y = 308, dificultad, cantidadPalabras;
  char **palabras;
  while (pantalla == 2)
  {
    pantalla2(fuenteRespuesta, background, assets, x, y);
    al_wait_for_event(eventos, &evento);
    if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
      pantalla = 0;
    if(evento.type == ALLEGRO_EVENT_KEY_CHAR && evento.keyboard.keycode == ALLEGRO_KEY_UP && y > 102)
      y-=206;
    if(evento.type == ALLEGRO_EVENT_KEY_CHAR && evento.keyboard.keycode == ALLEGRO_KEY_DOWN && y < 514)
      y+=206;
    if(evento.type == ALLEGRO_EVENT_KEY_CHAR && evento.keyboard.keycode == ALLEGRO_KEY_Z){
      dificultad = (y + 104) / 206;
      pantalla = 3;
    }
    al_rest(1.0/30);
  }
  palabras = obtienePalabras(dificultad, &cantidadPalabras);

  
  
  for(int i = 0; i < cantidadPalabras; i++)
    free(palabras[i]);
  free(palabras);
  al_destroy_event_queue(eventos);
  al_destroy_display(disp);
  al_destroy_bitmap(portada);
  al_destroy_bitmap(background);
  al_destroy_bitmap(assets);
  al_destroy_font(fuenteTexto);
  al_destroy_font(fuenteRespuesta);
  al_destroy_font(fuentePalabras);
}