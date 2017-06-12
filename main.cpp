#include <allegro.h>
#include "inicia.h"
#define MAXFILAS 20
#define MAXCOLS 31
BITMAP *buffer;
BITMAP *roca;
char mapa[MAXFILAS][MAXCOLS] =
{
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "X           XXXXX            X",
    "X XXX XXXXX XXXXX XXXXX XXX  X",
    "X XXX XXXXX XXXXX XXXXX XXX  X",
    "X                            X",
    "X XXX XX XXXXXXXXXXX XX XXX  X",
    "X     XX     XXX     XX      X",
    "X XXX XXXXXX XXX XXXXXX XXX  X",
    "X XXX XX             XX XXX  X",
    "      XX XXXXXXXXXXX XX       ",
    "X XXX XX XXXXXXXXXXX XX XXX  X",
    "X XXX XX             XX XXX  X",
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
        }
    }
}

void pantalla()
{
    blit(buffer, screen , 0 , 0, 0 , 0, 900, 600);
}

int main ()
{
    inicia_allegro(900,600);
    inicia_audio(70,70);

    buffer = create_bitmap(900,600);
    roca = load_bitmap("roca.bmp",NULL);
    clear_to_color(buffer, 0x999999);

    while(!key[KEY_ESC])
    {
      dibujar_mapa();
      pantalla();
    }




    return 0;
}
END_OF_MAIN ()


