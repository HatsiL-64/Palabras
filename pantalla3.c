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
//Busca una string especifica en un arreglo de strings. Requiere como primer parametro la string a buscar, el segundo
//parametro es donde la busca y el ultimo parametro es la medida del arreglo donde se buscara la string.
//Devuelve la posicion en el arreglo donde aparece la string buscada, si no esta devuelve -1. Solo devuelve la primera aparicion
int includesc(char* str, char** array, int length){
  int indice;
  for(int i = 0; i < length; i++){
    if(strcmp(str, array[i]) == 0){
      indice = i;
      return indice;
    }
  }
  return 0;
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
  
  int dificultad = 3, cantidadPalabras;
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
  char respuesta[21];
  int cantidadLetras = 0, letra, tempi, *indiceEncontradas = NULL, palabrasEncontradas = 0, try = 0;
  respuesta[0] = '\0';
  indiceEncontradas = malloc(sizeof(int));
  while(pantalla == 4){
    pantalla4(fuenteRespuesta, indiceEncontradas, palabrasEncontradas, respuesta);

    al_wait_for_event(eventos, &evento);
    if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
      pantalla = 0;
    if(evento.type == ALLEGRO_EVENT_KEY_CHAR && cantidadLetras <= 20){
      letra = evento.keyboard.keycode;
      if(isalpha(letra) != 0){
        respuesta[cantidadLetras] = letra;
        respuesta[cantidadLetras + 1] = '\0';
        cantidadLetras++;
      }
      if(letra == ALLEGRO_KEY_BACKSPACE && cantidadLetras > 0){
        respuesta[cantidadLetras - 1] = '\0';
        cantidadLetras--;
      }
      if(letra == ALLEGRO_KEY_ENTER && cantidadLetras > 0){
        try++;
        tempi = includesc(respuesta, palabras, cantidadPalabras);
        if(tempi != -1){
          indiceEncontradas[palabrasEncontradas] = tempi;
          palabrasEncontradas++;
          indiceEncontradas = realloc(indiceEncontradas, sizeof(int*) * (palabrasEncontradas + 1));
          //MUESTRA BREVEMENTE ALGO QUE DIGA QUE ACERTASTE
        }
        else{
          //MUESTRA ALGO QUE DIGA QUE TE EQUIVOCASTE
        }
        respuesta[0] = '\0';
        cantidadLetras = 0;
      }
    }
    if(try == cantidadPalabras){
      pantalla = 5;
    }
  }
  for(int i = 0; i < cantidadPalabras; i++)
    free(palabras[i]);
  free(palabras);
  free(indiceEncontradas);
  al_destroy_event_queue(eventos);
  al_destroy_display(disp);
  al_destroy_bitmap(portada);
  //al_destroy_bitmap(background);
  //al_destroy_bitmap(assets);
  al_destroy_timer(tempo);
  al_destroy_font(fuenteTexto);
  al_destroy_font(fuenteRespuesta);
  al_destroy_font(fuentePalabras);
}