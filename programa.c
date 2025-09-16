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

//Escanea las reglas
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
//Requiere un array de strings, la longitud de este array y una palabra que buscara
//devuelve el indice en el array de strings donde se encuentra la palabra buscada, si no esta, devuelve -1
//si la palabra buscada esta mas de una vez en el array, solo devuelve la primera aparicion.
int includesC(char **arrayS, int length, char *str)
{
  int coincidencia = -1;
  for (int i = 0; i < length; i++) {
    if(strcmp(arrayS[i], str) == 0){
      coincidencia = i;
      break;
    }
  }
  return coincidencia;
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

int main()
{
  ALLEGRO_DISPLAY *disp;
  ALLEGRO_EVENT_QUEUE *eventos;
  ALLEGRO_EVENT evento;
  ALLEGRO_FONT *fuenteTexto, *fuentePalabras, *fuenteRespuesta;
  ALLEGRO_TIMER *tempo;
  ALLEGRO_BITMAP *portada = NULL;
  ALLEGRO_COLOR blanco = al_map_rgb(255, 255, 255);
  ALLEGRO_COLOR negro = al_map_rgb(0, 0, 0);

  srand(time(NULL));

  //Archivos
  char temp, *stemp, respuesta[21];
  int *indice, nLineas = 0;
  char **palabras = malloc(sizeof(char*) * 8);
  respuesta[0] = '\0';

  FILE *archivo = NULL;
  archivo = fopen("src/palabras.txt" ,"r");
  if(archivo == NULL){
    return -1;
  }

  while ((temp = fgetc(archivo)) != EOF) { //Cuenta la cantidad de lineas 
    if(temp == '\n')
      nLineas++;
  }

  indice = malloc(sizeof(int) * nLineas);
  for(int i = 0; i < nLineas; i++){
    indice[i] = i;
  }

  desorden(indice, nLineas);
  sort(indice, 8);

  int k = 0;
  rewind(archivo);
  for(int i = 0; i < nLineas; i++){
    if(k < 8){
      if(i == indice[k]){
        palabras[k] = scanif(archivo);
        k++;
      }
      else {
        stemp = scanif(archivo);
      }
    }
  }
  fclose(archivo);

  for(int i = 0; i < 8; i++)
    printf("%s \n", palabras[i]);

  if(!al_init()){
    return -1;
  }
  if(!al_init_image_addon()){
    return -1;
  }
  if(!al_init_primitives_addon()){
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

  disp = al_create_display(1280, 720);
  al_set_window_title(disp, "Palabras");
  
  tempo = al_create_timer(1/4);
  
  portada = al_load_bitmap("statics/img/PORTADA.png");

  eventos = al_create_event_queue();
  al_register_event_source(eventos, al_get_display_event_source(disp));
  al_register_event_source(eventos, al_get_keyboard_event_source());
  al_register_event_source(eventos, al_get_timer_event_source(tempo));
  
  fuenteTexto = al_load_font("statics/fonts/Swansea-q3pd.ttf", 32, 0);
  fuentePalabras = al_load_font("statics/fonts/SwanseaBoldItalic-p3Dv.ttf", 100, 0);
  fuenteRespuesta = al_load_font("statics/fonts/SwanseaItalic-AwqD.ttf", 50, 0);
  
  //Portada. Esto de seguro lo voy a borrar pero me hace ilusion ver mi portada
  bool bucle = true;
  while(bucle)
  {
    al_draw_bitmap(portada, 0, 0, 0);
    al_draw_text(fuenteTexto, blanco, 640, 650, ALLEGRO_ALIGN_CENTER, "Presiona E para empezar");
    al_flip_display();

    al_wait_for_event(eventos, &evento);
    if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
      bucle = false;
    if(evento.type == ALLEGRO_EVENT_KEY_CHAR && evento.keyboard.keycode == ALLEGRO_KEY_E)
      bucle = false;
    al_rest(1/40);
  }
  
  bucle = true;
  k = 0;
  al_start_timer(tempo);
  while(bucle)
  {
    al_clear_to_color(negro);
    al_draw_text(fuentePalabras, blanco, 640, 310, ALLEGRO_ALIGN_CENTER, palabras[k]);
    al_flip_display();
    
    al_wait_for_event(eventos, &evento);
    if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
      bucle = false;
    if(evento.type == ALLEGRO_EVENT_TIMER){
      k++;
      if(k == 8)
        bucle = false;
    }
    al_rest(1/60);
  }

  bucle = true;
  char caracter;
  int aciertos = 0, nRespuestas = 0;
  k = 0;

  while (bucle) {
    al_clear_to_color(negro);
    al_draw_filled_rectangle(0, 720, 1280, 620, blanco);
    al_draw_text(fuenteRespuesta, negro, 640, 680, ALLEGRO_ALIGN_CENTER, respuesta);
    al_flip_display();

    al_wait_for_event(eventos, &evento);
    if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
      bucle = false;
    if(evento.type == ALLEGRO_EVENT_KEY_CHAR){
      caracter = evento.keyboard.unichar;
      if( k < 20){
        respuesta[k] = caracter;
        respuesta[k+1] = '\0';
        k++;
      }
      if((caracter == 8 || caracter == 127 )&& k > 0){
        k--;
        respuesta[k] = '\0';
      }
      if(caracter == 13){
        nRespuestas++;
        /*if((indice[nRespuestas] = includesC(palabras, respuesta) >= 0){

        }*/
        respuesta[0] = '\0';
        k = 0;
      }
      if(nRespuestas == 8)
        bucle = false;
    }
    al_rest(1/60);
  }

  for(int i = 0; i < 8; i++)
    free(palabras[i]);
  free(palabras);
  free(indice);
  free(stemp);

  al_destroy_bitmap(portada);
  al_destroy_font(fuenteTexto);
  al_destroy_font(fuentePalabras);
  al_destroy_font(fuenteRespuesta);
  al_destroy_event_queue(eventos);
  al_destroy_timer(tempo);
  al_destroy_display(disp);
  return 0;
}
