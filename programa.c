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

void instrucciones()
{
  ALLEGRO_FONT *fuenteChica;
  ALLEGRO_EVENT_QUEUE *colaInstrucciones;
  ALLEGRO_EVENT eventoInstrucciones;
  ALLEGRO_COLOR negro = al_map_rgb(0, 0, 0);
  ALLEGRO_COLOR blanco = al_map_rgb(255, 255, 255);
  
  colaInstrucciones = al_create_event_queue();
  al_register_event_source(colaInstrucciones, al_get_keyboard_event_source());

  fuenteChica = al_load_ttf_font("statics/fonts/Swansea-q3pd.ttf", 15, 0);
  
  int run = 1, letras, lineasi;
  char *contenido, **lineas;
  FILE *reglas = NULL;
  reglas = fopen("src/reglas.txt", "r");
  if(reglas == NULL){
    printf("Fallo al abrir el archivo");
    return;
  }

  fseek(reglas, 0, SEEK_END);
  letras = ftell(reglas);
  rewind(reglas);
  
  contenido = malloc(letras + 1);
  fread(contenido, 1, letras, reglas);
  lineas = split(contenido, "\n", &lineasi);
  while (run)
  {
    al_draw_filled_rounded_rectangle(270, 210, 1010, 480, 10, 10, negro);
    for(int i = 0; i < lineasi; i++)
      al_draw_text(fuenteChica, blanco, 290, 220 + 20 * (i + 1), ALLEGRO_ALIGN_LEFT, lineas[i]);
    al_flip_display();

    al_wait_for_event(colaInstrucciones, &eventoInstrucciones);
    if(eventoInstrucciones.type == ALLEGRO_EVENT_KEY_DOWN && eventoInstrucciones.keyboard.keycode == ALLEGRO_KEY_P)
      run = 0;
  }
  
  fclose(reglas);
  for(int i = 0; i < lineasi; i++){
    free(lineas[i]);
  }
  free(lineas);
  free(contenido);
  al_destroy_event_queue(colaInstrucciones);
}

void pantalla1(ALLEGRO_BITMAP *fondo, ALLEGRO_FONT *font)
{
  ALLEGRO_COLOR negro = al_map_rgb(0, 0, 0);
  ALLEGRO_COLOR blanco = al_map_rgb(255, 255, 255);
  al_draw_bitmap(fondo, 0, 0, 0);
  al_draw_text(font, blanco, 640, 605, ALLEGRO_ALIGN_CENTER, "Presiona E para seguir");
  al_draw_text(font, blanco, 640, 645, ALLEGRO_ALIGN_CENTER, "Presiona P para mas informacion");
  al_flip_display();
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
  ALLEGRO_TIMER *tempo;

  int pantalla = 1;

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

  while (pantalla == 1)
  {
    pantalla1(portada, fuenteTexto);

    al_wait_for_event(eventos, &evento);
    if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
      pantalla = 0;
    if(evento.type == ALLEGRO_EVENT_KEY_CHAR && evento.keyboard.keycode == ALLEGRO_KEY_E)
      pantalla = 2;
    if(evento.type == ALLEGRO_EVENT_KEY_CHAR && evento.keyboard.keycode == ALLEGRO_KEY_P)
    {
      instrucciones();
      al_flush_event_queue(eventos);
    }
    al_rest(1.0/30);
  }

  background = al_load_bitmap("statics/img/bg.png");
  assets = al_load_bitmap("statics/img/flecha.png");
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
  al_destroy_timer(tempo);
  al_destroy_bitmap(portada);
  al_destroy_bitmap(background);
  al_destroy_bitmap(assets);
  al_destroy_font(fuenteTexto);
  al_destroy_font(fuenteRespuesta);
  al_destroy_font(fuentePalabras);
}