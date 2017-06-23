#include <allegro.h>
#include "inicia.h"
#include <cstdlib>
#define MAXFILAS 20
#define MAXCOLS 31
BITMAP *buffer;
BITMAP *roca;
BITMAP *pacbmp;
BITMAP *pacman;
BITMAP *comida;
BITMAP *enemigobmp;
BITMAP *enemigo;
BITMAP *muertebmp;

int dir = 4;
int px = 30*14, py = 30*17;
int fdir = 0 , _x = 30 * 14 , _y = 30 * 14;
int anteriorpx , anteriorpy;


char mapa[MAXFILAS][MAXCOLS] =
{
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "X      o o  XXXXX  o o       X",
    "X XXX XXXXX XXXXX XXXXX XXX  X",
    "X XXX XXXXX XXXXX XXXXX XXX  X",
    "X                            X",
    "X XXX XX XXXXXXXXXXX XX XXX  X",
    "X     XX     XXX     XX      X",
    "X XXX XXXXXX XXX XXXXXX XXX  X",
    "X XXX XX o         o XX XXX  X",
    "      XX XXXXXXXXXXX XX       ",
    "X XXX XX XXXXXXXXXXX XX XXX  X",
    "X XXX XX o         o XX XXX  X",
    "X XXX XXXXXX XXX XXXXXX XXX  X",
    "X     XX     XXX     XX      X",
    "X XXX XX XXXXXXXXXXX XX XXX  X",
    "X XXX                   XXX  X",
    "X XXX XXXX XXXXXXXX XXX XXX  X",
    "X XXX XXXX          XXX XXX  X",
    "X          XXXXXXXX          X",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
};

void dibujar_mapa()
{
    int row, col;

    for(row = 0 ; row < MAXFILAS; ++row)
    {
        for(col = 0; col < MAXCOLS; ++col)
        {
            if(mapa[row][col] == 'X')
            {
                draw_sprite(buffer, roca , col*30, row*30 );  // el *30 sale del tamaño de la imagen
            }
            else if(mapa[row][col] == 'o')
            {
                draw_sprite(buffer, comida , col*30, row*30 );
                if(py/30 == row && px/30 == col) mapa[row][col] = ' ';
            }
        }
    }
}

void pantalla()
{
    blit(buffer, screen , 0 , 0, 0 , 0, 900, 600);
}

void dibujar_personaje_abriendo()
{
    blit(pacbmp,pacman,dir*33,0,0,0,33,33);
    draw_sprite(buffer,pacman,px,py);
}

void dibujar_personaje_cerrando()
{
    blit(pacbmp,pacman,4*33,0,0,0,33,33);
    draw_sprite(buffer,pacman,px,py);
}

bool game_over()
{
    int row, col;
     for(row = 0 ; row < MAXFILAS; ++row)
        for(col = 0; col < MAXCOLS; ++col)
             if(mapa[row][col] == 'o') return true;
    return false;
}


void mover_pacman(int dir)
{
    if(dir == 0  )
    {
        if(mapa[py/30][(px-30)/30] != 'X') px -= 30;
        else dir = 4;
    }
    else if(dir == 1)
    {
        if(mapa[py/30][(px+30)/30] != 'X') px += 30;
        else dir = 4;
    }
    else if(dir == 2)
    {
        if(mapa[(py-30)/30][px/30] != 'X') py -= 30;
        else dir = 4;

    }
    else if(dir == 3)
    {
        if(mapa[(py+30)/30][px/30] != 'X') py += 30;
        else dir =  4;
    }

    //rutina para atajos

    if( px <= -30) px = 870;
    else if (px >= 875) px = -30;



}

void dibujar_fantasma()
{
    blit(enemigobmp,enemigo,0,0,0,0,30,30);
    draw_sprite(buffer,enemigo,_x,_y);
}

void choque_pacman()
{
    if( py == _y && px == _x || anteriorpx == _x && anteriorpy == _y)
    {
        for(int i = 0; i <= 5; ++i)
        {
            clear(pacman);
            clear(buffer);
            dibujar_mapa();

            blit(muertebmp,pacman,i*33,0,0,0,33,33);
            draw_sprite(buffer,pacman,px,py);
            pantalla();
            rest(80);
        }

        px = 30*14, py = 30*17 , dir = 4;
    }
}

void fantasma()
{
    dibujar_fantasma();
    choque_pacman();
    if(fdir == 0)
    {
        if(mapa[_y/30][(_x - 30)/30] != 'X') _x -= 30;
        else fdir = rand()%4;
    }

    if(fdir == 1)
    {
        if(mapa[_y/30][(_x + 30)/30] != 'X') _x += 30;
        else fdir = rand()%4;
    }
    if(fdir == 2)
    {
        if(mapa[(_y-30)/30][_x/30] != 'X') _y -= 30;
        else fdir = rand()%4;
    }
       if(fdir == 3)
    {
        if(mapa[(_y+30)/30][_x/30] != 'X') _y += 30;
        else fdir = rand()%4;
    }

    // rutina atajos

    if( _x <= -30) _x = 870;
    else if (_x >= 875) _x = -30;


}

int main ()
{
    inicia_allegro(900,600);
    inicia_audio(70,70);

    buffer = create_bitmap(900,600);
    roca = load_bitmap("roca.bmp",NULL);
    pacbmp = load_bitmap("pacman.bmp",NULL);
    pacman =  create_bitmap(33,33); // los datos salen de las mediciones de las imagenes
    comida = load_bitmap("Comida.bmp", NULL);
    enemigo = create_bitmap(30,30);
    enemigobmp = load_bitmap("enemigo.bmp", NULL);
    muertebmp =  load_bitmap("muerte.bmp", NULL);
    clear_to_color(buffer, 0x999999);

    while(!key[KEY_ESC] && game_over())
    {
       anteriorpx = px;
       anteriorpy = py;
      if(key[KEY_RIGHT]) dir = 1;
      else if(key[KEY_LEFT]) dir = 0;
      else if(key[KEY_UP]) dir = 2;
      else if(key[KEY_DOWN]) dir = 3;
      mover_pacman(dir);
      clear(buffer);
      dibujar_mapa();
      dibujar_personaje_abriendo();
      fantasma();
      pantalla();
      rest(70);
      dibujar_personaje_cerrando();
      pantalla();
      rest(90);

    }




    return 0;
}
END_OF_MAIN ()


