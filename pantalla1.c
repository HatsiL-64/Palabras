//La primera pantalla es unicamente una imagen de inicio y donde puedes ver las instrucciones del programa
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

void pantalla1(ALLEGRO_BITMAP *fondo, ALLEGRO_FONT *font)
{
  ALLEGRO_COLOR negro = al_map_rgb(0, 0, 0);
  ALLEGRO_COLOR blanco = al_map_rgb(255, 255, 255);
  al_draw_bitmap(fondo, 0, 0, 0);
  al_draw_text(font, blanco, 640, 605, ALLEGRO_ALIGN_CENTER, "Presiona E para seguir");
  al_draw_text(font, blanco, 640, 645, ALLEGRO_ALIGN_CENTER, "Presiona P para mas informacion");
  al_flip_display();
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
  ALLEGRO_BITMAP *portada, *assets;

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

  al_destroy_event_queue(eventos);
  al_destroy_display(disp);
  al_destroy_bitmap(portada);
  al_destroy_font(fuenteTexto);
  al_destroy_font(fuenteRespuesta);
  al_destroy_font(fuentePalabras);
}