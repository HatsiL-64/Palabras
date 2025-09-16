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
#include "biblioteca/funjs.h"

#define XMAX 1280
#define YMAX 720

char* normalize( char* str ){
  
}

int includesc(char* str, char** array, int length){
  int indice;
  for(int i = 0; i < length; i++){
    if(strcmp(lower(str), lower(array[i])) == 0){
      indice = i;
      return indice;
    }
  }
  return -1;
}

char **obtienePalabras(int opc, int *palabrasn){
  if(opc == 1)
    *palabrasn = 6;
  if(opc == 2)
    *palabrasn = 8;
  if(opc == 3)
    *palabrasn = 12;

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

void pantalla4(ALLEGRO_FONT *font, ALLEGRO_BITMAP *background, char* respuesta, char** respuestas, int palabras, int palabrasn){
  int y = (YMAX - 100) / (palabrasn + 1);
  al_clear_to_color(al_map_rgb(255, 255, 255));
  al_draw_bitmap(background, 0, 0, 0);
  al_draw_filled_rectangle(0, 620, 1280, 720, al_map_rgb(0, 0, 0));
  al_draw_text(font, al_map_rgb(255, 255, 255), 640, 645, ALLEGRO_ALIGN_CENTER, respuesta);
  //Conforme mas adivines, se proyectan las respuestas.
  for(int i = 0; i < palabras; i++){
    if(i < palabras / 2)//<=
      al_draw_text(font, al_map_rgb(0, 0, 0), 50, y+(y*i*2), ALLEGRO_ALIGN_LEFT, respuestas[i]);
    else
      al_draw_text(font, al_map_rgb(0, 0, 0), 1230, y+(y*2*(i - palabras / 2)), ALLEGRO_ALIGN_RIGHT, respuestas[i]);
  }
  al_flip_display();
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
  ALLEGRO_TIMER *tempo;

  int pantalla = 3;

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
  
  int dificultad = 1, cantidadPalabras;
  char **palabras;
  palabras = obtienePalabras(dificultad, &cantidadPalabras);
  // A partir de aqui copiar al codigo original
  int palabra = 0;
  tempo = al_create_timer(1.0/dificultad); //Si esta muy facil puede ser al_create_timer(1/ pow(2, dificultad - 1));
  al_register_event_source(eventos, al_get_timer_event_source(tempo));
  al_start_timer(tempo);
  while (pantalla == 3)
  {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(fuentePalabras, al_map_rgb(255, 255, 255), 640, 310, ALLEGRO_ALIGN_CENTER, palabras[palabra]);
    al_flip_display();

    al_wait_for_event(eventos, &evento);
    if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
      pantalla = 0;
    if(evento.type == ALLEGRO_EVENT_TIMER){
      palabra++;
      if(palabra == cantidadPalabras){
        al_stop_timer(tempo);
        pantalla = 4;
      }
    }
  }
  for(int i = 0; i < cantidadPalabras; i++)//------------------------------------------------------------------------------
    printf("%s \n", palabras[i]);
  background = al_load_bitmap("statics/img/tragamonedas.png");
  char respuesta[21], **respuestas = NULL;
  int cantidadLetras = 0, letra, palabrasEncontradas = 0, try = 0;
  respuesta[0] = '\0';
  while(pantalla == 4){
    pantalla4(fuenteRespuesta, background, respuesta, respuestas, palabrasEncontradas, cantidadPalabras);

    al_wait_for_event(eventos, &evento);
    if(try == cantidadPalabras){
      pantalla = 5;
      al_rest(1);
    }
    if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
      pantalla = 0;
    if(evento.type == ALLEGRO_EVENT_KEY_CHAR){
      letra = evento.keyboard.unichar;
      if(isalpha(letra) != 0 && cantidadLetras <= 20){
        respuesta[cantidadLetras] = letra;
        respuesta[cantidadLetras + 1] = '\0';
        cantidadLetras++;
      }
      if(evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && cantidadLetras > 0){
        respuesta[cantidadLetras - 1] = '\0';
        cantidadLetras--;
      }
      if(evento.keyboard.keycode == ALLEGRO_KEY_ENTER && cantidadLetras > 0){
        try++;
        if(includesc(respuesta, palabras, cantidadPalabras) != -1){
          respuestas = realloc(respuestas, sizeof(char*) * (palabrasEncontradas+1)); 
          respuestas[palabrasEncontradas] = mstrdup(respuesta);
          palabrasEncontradas++;
          //MUESTRA BREVEMENTE ALGO QUE DIGA QUE ACERTASTE
        }
        else{
          //MUESTRA ALGO QUE DIGA QUE TE EQUIVOCASTE
        }
        respuesta[0] = '\0';
        cantidadLetras = 0;
      }
    }
  }
  for(int i = 0; i < cantidadPalabras; i++)
    free(palabras[i]);
  free(palabras);
  for(int i = 0; i < palabrasEncontradas; i++)
    free(respuestas[i]);
  free(respuestas);
  al_destroy_event_queue(eventos);
  al_destroy_display(disp);
  al_destroy_bitmap(portada);
  al_destroy_bitmap(background);
  //al_destroy_bitmap(assets);
  al_destroy_timer(tempo);
  al_destroy_font(fuenteTexto);
  al_destroy_font(fuenteRespuesta);
  al_destroy_font(fuentePalabras);
}