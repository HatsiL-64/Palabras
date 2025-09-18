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
#include ".biblioteca/funjs.h"

#define XMAX 1280
#define YMAX 720

int includesc(char* str, char** array, int length){
  if(array == NULL || str == NULL)
    return -1;
  int indice;
  char *lowStr , *lowArr;
  for(int i = 0; i < length; i++){
    lowStr = lower(str);
    lowArr = lower(array[i]);
    if(strcmp(lower(str), lower(array[i])) == 0){
      indice = i;
      free(lowStr);
      free(lowArr);
      return indice;
    }
    free(lowStr);
    free(lowArr);
  }
  return -1;
}
//Esta funcion es hermosa, lastima que el usar el mouse dio muchos problemas. Me apegare a usar el teclado
/*int botonCirculo(double cx, double cy, double r, double mx, double my){
  double x = (cx - mx) * (cx - mx);
  double y = (cy - my) * (cy - my);
  double coalicion = x + y;
  //double coalicion = pow(cx - mx, 2) + pow(cy - my, 2);
  if(coalicion <= r * r)
    return 1;
  else
    return 0;
}*/

void instrucciones(ALLEGRO_BITMAP *cuadroTexto, ALLEGRO_BITMAP *fondo)
{
  ALLEGRO_FONT *fuenteChica;
  ALLEGRO_EVENT_QUEUE *colaInstrucciones;
  ALLEGRO_EVENT eventoInstrucciones;
  
  colaInstrucciones = al_create_event_queue();
  al_register_event_source(colaInstrucciones, al_get_keyboard_event_source());

  fuenteChica = al_load_ttf_font("statics/fonts/Swansea-q3pd.ttf", 15, 0);
  
  int run = 1, letras, lineasi;
  char *contenido, **lineas;
  FILE *reglas = NULL;
  reglas = fopen("src/reglas.txt", "r");
  if(reglas == NULL){
    return;
  }

  fseek(reglas, 0, SEEK_END);
  letras = ftell(reglas);
  rewind(reglas);
  
  contenido = malloc(letras + 1);
  fread(contenido, 1, letras, reglas);
  lineas = split(contenido, "\n", &lineasi);
  al_draw_bitmap(fondo, 0, 0, 0);
  al_draw_scaled_bitmap(cuadroTexto, 0, 0, 200, 200, 200, 190, 860, 290, 0);
  for(int i = 0; i < lineasi; i++)
    al_draw_text(fuenteChica, al_map_rgb(255, 255, 255), 290, 220 + 20 * (i + 1), ALLEGRO_ALIGN_LEFT, lineas[i]);
  al_flip_display();
  
  while (run)
  {
    al_wait_for_event(colaInstrucciones, &eventoInstrucciones);
    if(eventoInstrucciones.type == ALLEGRO_EVENT_KEY_CHAR && eventoInstrucciones.keyboard.keycode == ALLEGRO_KEY_P)
      run = 0;
    al_rest(1.0/30);
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
  al_draw_text(font, blanco, 640, 129.5, ALLEGRO_ALIGN_CENTER, "Fácil");
  al_draw_filled_rounded_rectangle(557.2, 308, 722.8, 411, 10, 10, negro);
  al_draw_text(font, blanco, 640, 334.5, ALLEGRO_ALIGN_CENTER, "Medio");
  al_draw_filled_rounded_rectangle(557.2, 514, 722.8, 617, 10, 10, negro);
  al_draw_text(font, blanco, 640, 540.5, ALLEGRO_ALIGN_CENTER, "Dífcil");
  al_draw_scaled_bitmap(extra, 0, 0, al_get_bitmap_width(extra), al_get_bitmap_height(extra), x - 50, y - 50, 225, 200, 0);
  al_flip_display();
}

void pantalla4(ALLEGRO_FONT *font, ALLEGRO_BITMAP *background, char* respuesta, char** respuestas, int palabras, int palabrasn, int try){
  ALLEGRO_COLOR negro = al_map_rgb(0, 0, 0);
  int y = (YMAX - 100) / (palabrasn + 1);
  char intentos[6];
  sprintf(intentos, "%i/%i", try, palabrasn);
  al_clear_to_color(al_map_rgb(255, 255, 255));
  al_draw_bitmap(background, 0, 0, 0);
  al_draw_filled_rectangle(0, 620, 1280, 720, negro);
  al_draw_text(font, al_map_rgb(255, 255, 255), 640, 645, ALLEGRO_ALIGN_CENTER, respuesta);
  al_draw_text(font, negro, 640, 50, ALLEGRO_ALIGN_CENTER, intentos);
  //al_draw_text(font, al_map_rgb(255, 255, 255), 580, 560, ALLEGRO_ALIGN_CENTER, "ESC para volver");
  //Conforme mas adivines, se proyectan las respuestas.
  for(int i = 0; i < palabras; i++){
    if(i < palabras / 2)//<=
      al_draw_text(font, negro, 50, y+(y*i*2), ALLEGRO_ALIGN_LEFT, respuestas[i]);
    else
      al_draw_text(font, negro, 1230, y+(y*2*(i - palabras / 2)), ALLEGRO_ALIGN_RIGHT, respuestas[i]);
  }
  al_flip_display();
}

void pantalla5(ALLEGRO_FONT *font, ALLEGRO_BITMAP *rata, int aciertos, int palabras, char** respuestas){
  char resultado[50];
  int y = (YMAX - 100) / (palabras + 1);
  ALLEGRO_COLOR blanco = al_map_rgb(255, 255, 255);
  sprintf(resultado, "Tuviste %i aciertos de %i", aciertos, palabras);
  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_draw_bitmap(rata, 452.5, 100, 0);//y = 50
  al_draw_text(font, blanco, 640, 545, ALLEGRO_ALIGN_CENTER, resultado);
  al_draw_text(font, blanco, 640, 620, ALLEGRO_ALIGN_CENTER, "Presiona E para salir o R para volver");
  for(int i = 0; i < palabras; i++){
    if(i < palabras / 2)
      al_draw_text(font, blanco, 50, y+(y*i*2), ALLEGRO_ALIGN_LEFT, respuestas[i]);
    else
      al_draw_text(font, blanco, 1230, y+(y*2*(i - palabras / 2)), ALLEGRO_ALIGN_RIGHT, respuestas[i]);
  }
  al_flip_display();
}

//1 = ganar. 2 = fallo
void animacionTragamonedas(int opc){
  ALLEGRO_BITMAP *simbolos = al_load_bitmap("statics/img/slotSpriteSheet.png");
  ALLEGRO_TIMER *animacion;
  ALLEGRO_EVENT event;
  ALLEGRO_EVENT_QUEUE *queue;
  int run, fotogramas, x = 0, y = 0;
  run = 1;
  fotogramas = 0;
  animacion = al_create_timer(1.0/30);
  queue = al_create_event_queue();
  al_register_event_source(queue, al_get_timer_event_source(animacion));
  al_start_timer(animacion);
  while (run){
    al_wait_for_event(queue, &event);
    if(event.type == ALLEGRO_EVENT_TIMER){
      if(fotogramas % 4 == 0 && fotogramas != 0){
        y = 0;
        x+=65;
        if(fotogramas % 16 == 0 && fotogramas != 0){
          x = 0;
        } 
      }
      if(fotogramas == 30 && opc == 1){
        x = 195;
        y = 195;
        run = 0;
      }
      if(fotogramas == 30 && opc == 2){
        x = 195;
        y = 130;
        run = 0;
      }
    }
    al_draw_scaled_bitmap(simbolos, x, y, 65, 65, 500, 250, 100, 100, 0);
    al_flip_display();
    fotogramas++;
    y+=65;
  }
  al_rest(0.5);
  al_destroy_event_queue(queue);
  al_destroy_timer(animacion);
  al_destroy_bitmap(simbolos);
}

char **obtienePalabras(int opc, int *palabrasn){
  if(opc == 1)
    *palabrasn = 4;
  if(opc == 2)
    *palabrasn = 8;
  if(opc == 3)
    *palabrasn = 10;

  char temp, *stemp = NULL;
  int nLineas = 0, *indice;
  char **palabras = malloc(sizeof(char*) * (*palabrasn));
  FILE *archPalabras = NULL;
  archPalabras = fopen("src/.palabras.txt", "r");
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
        free(stemp);
      }
    }
  }

  fclose(archPalabras);
  free(indice);
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
  ALLEGRO_BITMAP *assets, *background, *textHolder;
  ALLEGRO_TIMER *tempo;

  int pantalla = 1;//mx, my
  int x = 800, y = 308, dificultad, cantidadPalabras, palabra, cantidadLetras, letra, palabrasEncontradas, try;
  char **palabras = NULL;
  char respuesta[21], **respuestas = NULL;

  srand(time(NULL));

  disp = al_create_display(XMAX, YMAX);
  al_set_window_title(disp, "Palabras");

  background = al_load_bitmap("statics/img/PORTADA.png");
  textHolder = al_load_bitmap("statics/img/textholder.png");

  fuenteTexto = al_load_ttf_font("statics/fonts/Swansea-q3pd.ttf", 30, 0);
  fuenteRespuesta = al_load_ttf_font("statics/fonts/SwanseaBold-D0ox.ttf", 50, 0);
  fuentePalabras = al_load_ttf_font("statics/fonts/SwanseaBoldItalic-p3Dv.ttf", 100, 0);

  eventos = al_create_event_queue();
  al_register_event_source(eventos, al_get_display_event_source(disp));
  al_register_event_source(eventos, al_get_keyboard_event_source());

  while (pantalla < 6 && pantalla > 0)
  {
    while (pantalla == 1)
    {
      pantalla1(background, fuenteTexto);

      al_wait_for_event(eventos, &evento);
      if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        pantalla = 0;
      if(evento.type == ALLEGRO_EVENT_KEY_CHAR && evento.keyboard.keycode == ALLEGRO_KEY_E)
        pantalla = 2;
      if(evento.type == ALLEGRO_EVENT_KEY_CHAR && evento.keyboard.keycode == ALLEGRO_KEY_P){
        instrucciones(textHolder, background);
        al_flush_event_queue(eventos);
      }
      al_rest(1.0 / 30);
    }

    al_destroy_bitmap(background);
    background = al_load_bitmap("statics/img/bg.png");
    assets = al_load_bitmap("statics/img/flecha.png");
    
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
        palabras = obtienePalabras(dificultad, &cantidadPalabras);
      }
      al_rest(1.0/30);
    }
    
    palabra = 0;
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
        if(palabra == cantidadPalabras)
          pantalla = 4;
      }
    }
    al_stop_timer(tempo);
    al_destroy_timer(tempo);

    al_destroy_bitmap(background);
    background = al_load_bitmap("statics/img/tragamonedas.png");
    try = 0;
    palabrasEncontradas = 0;
    cantidadLetras = 0;
    respuesta[0] = '\0';
    while(pantalla == 4){
      pantalla4(fuenteRespuesta, background, respuesta, respuestas, palabrasEncontradas, cantidadPalabras, try);

      al_wait_for_event(eventos, &evento);
      if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        pantalla = 0;
      if(evento.type == ALLEGRO_EVENT_KEY_CHAR){
        letra = evento.keyboard.unichar;
        if(isalpha(letra) != 0 && cantidadLetras < 20){
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
          if(includesc(respuesta, palabras, cantidadPalabras) != -1 && includesc(respuesta, respuestas, palabrasEncontradas) == -1){
            respuestas = realloc(respuestas, sizeof(char*) * (palabrasEncontradas+1)); 
            respuestas[palabrasEncontradas] = mstrdup(respuesta);
            palabrasEncontradas++;
            animacionTragamonedas(1);
            al_flush_event_queue(eventos);
          }
          else{
            //MUESTRA ALGO QUE DIGA QUE TE EQUIVOCASTE
            animacionTragamonedas(2);
            al_flush_event_queue(eventos);
          }
          respuesta[0] = '\0';
          cantidadLetras = 0;
        }
        if(evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
          pantalla = 7;
      }
      if(try == cantidadPalabras){
        pantalla = 5;
        al_rest(1);
      }
    }

    int imagen = rand() % 3 + 1;
    al_destroy_bitmap(assets);
    if(imagen == 1)
      assets = al_load_bitmap("statics/img/h.png");
    if(imagen == 2)
      assets = al_load_bitmap("statics/img/hugo.png");
    if(imagen == 3)
      assets = al_load_bitmap("statics/img/m.png");
    while(pantalla == 5){
      pantalla5(fuenteRespuesta, assets, palabrasEncontradas, cantidadPalabras, palabras);
      
      al_wait_for_event(eventos, &evento);
      if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
        pantalla = 6;
      }
      if(evento.type == ALLEGRO_EVENT_KEY_CHAR && evento.keyboard.keycode == ALLEGRO_KEY_E){
        pantalla = 6;
      }
      if(evento.type == ALLEGRO_EVENT_KEY_CHAR && evento.keyboard.keycode == ALLEGRO_KEY_R){
        pantalla = 2;
      }
    }
    if(pantalla == 7)
      pantalla = 2;
    if(palabras != NULL){
      for(int i = 0; i < cantidadPalabras; i++)
        free(palabras[i]);
      free(palabras);
      palabras = NULL;
    }
    if(respuestas != NULL){
      for(int i = 0; i < palabrasEncontradas; i++)
        free(respuestas[i]);
      free(respuestas); 
      respuestas = NULL;
    }
  }
  al_destroy_event_queue(eventos);
  al_destroy_display(disp);
  al_destroy_bitmap(background);
  al_destroy_bitmap(assets);
  al_destroy_bitmap(textHolder);
  al_destroy_font(fuenteTexto);
  al_destroy_font(fuenteRespuesta);
  al_destroy_font(fuentePalabras);
}
