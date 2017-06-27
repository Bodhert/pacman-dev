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
BITMAP *muertebmp;
MIDI *musica1;
SAMPLE *bolita;
SAMPLE *caminando;
SAMPLE *muerte;

int dir = 4;
int px = 30*14, py = 30*17;
int anteriorpx , anteriorpy;


char mapa[MAXFILAS][MAXCOLS] =
{
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "X      o o  XXXXX  o o       X",
    "X XXX XXXXX XXXXX XXXXX XXXX X",
    "X XXX XXXXX XXXXX XXXXX XXXX X",
    "X                            X",
    "X XXX XX XXXXXXXXXXX XX XXXX X",
    "X     XX     XXX     XX      X",
    "X XXX XXXXXX XXX XXXXXX XXXX X",
    "X XXX XX o         o XX XXXX X",
    "      XX XXXXXXXXXXX XX       ",
    "X XXX XX XXXXXXXXXXX XX XXXX X",
    "X XXX XX o         o XX XXXX X",
    "X XXX XXXXXX XXX XXXXXX XXXX X",
    "X     XX     XXX     XX      X",
    "X XXX XX XXXXXXXXXXX XX XXXX X",
    "X XXX                   XXXX X",
    "X XXX XXXX XXXXXXXX XXX XXXX X",
    "X XXX XXXX          XXX XXXX X",
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
                if(py/30 == row && px/30 == col)
                {
                    play_sample(bolita,100,150,1000,0); // hay un bug con comerse la bolita
                    mapa[row][col] = ' ';

                }
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


class fantasma
{

    BITMAP *enemigobmp;
    BITMAP *enemigo;
    int fdir, _x , _y;


public :
    fantasma(int x, int y); // constructor
    void dibujar_fantasma() const;
    void mover_fantasma();
    void choque_pacman();

};

fantasma::fantasma(int x, int y)
{
 _x = x; _y = y;
 fdir = rand()%4;
 enemigo = create_bitmap(30,30);
 enemigobmp = load_bitmap("enemigo.bmp", NULL);
}

void fantasma::dibujar_fantasma() const
{
        blit(enemigobmp,enemigo,0,0,0,0,30,30);
        draw_sprite(buffer,enemigo,_x,_y);
}

void fantasma::choque_pacman()
{
     if( py == _y && px == _x || anteriorpx == _x && anteriorpy == _y)
        {
            play_sample(muerte,100,150,1000,0);
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

void fantasma::mover_fantasma()
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
    musica1 = load_midi("mario.mid");
    bolita = load_wav("coin.wav");
    caminando = load_wav("jump.wav");
    muerte = load_wav("Muerte.wav");

    buffer = create_bitmap(900,600);
    roca = load_bitmap("roca.bmp",NULL);
    pacbmp = load_bitmap("pacman.bmp",NULL);
    pacman =  create_bitmap(33,33); // los datos salen de las mediciones de las imagenes
    comida = load_bitmap("Comida.bmp", NULL);
    muertebmp =  load_bitmap("muerte.bmp", NULL);
    clear_to_color(buffer, 0x999999);
    fantasma A(30*2,30*3);
    fantasma B(30*15,30*15);

    play_midi(musica1,1);
    while(!key[KEY_ESC] && game_over())
    {
    if( dir != 4 ) play_sample(caminando,100,150,1000,0);
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
      A.mover_fantasma();
      B.mover_fantasma();
      pantalla();
      rest(70);
      dibujar_personaje_cerrando();
      pantalla();
      rest(90);

    }




    return 0;
}
END_OF_MAIN ()


