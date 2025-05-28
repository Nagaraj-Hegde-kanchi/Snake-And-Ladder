#ifndef RAYLIB_H_
    #define RAYLIB_H_
    #include "raylib.h"
#endif
typedef struct Snakes
{
    Texture2D texture;
    Rectangle source;
    Rectangle dest;
    Vector2 origin;
} Snakes;

typedef struct Ladders{
    Texture2D texture;
    Rectangle source;
    Rectangle dest;
    Vector2 origin;
    int rotation;
}Ladders;

typedef struct Player
{
    Texture2D texture;
    Rectangle source;
    int previous_position;
    int position;
    int row;
    int column;
} Player;

typedef enum Turn{
    blue, green, red, yellow
}Turn;

typedef struct Die
{
    Texture2D texture;
    Rectangle source;
    Rectangle dest[4];
    Vector2 origin;

}Die;


int n =3;
const float GameSpeed[] = {0.02, 0.05, 0.1, 0.3, 0.5, 1};
int Gspeedptr = 0; 
static int i =0 ;           // this 'int i' is used in rolling animation of the dice.
int current_die_face = 1;
int dieWidth= 865;
int dieHeight=840;
Turn cur_turn;
Ladders ladder[4];
Die die;
Rectangle cells[10][10];
int screenWidth = 1200;
int screenHeight = 800;
int cellwidth = 64;
int cellheight = 64;
Texture2D board;
Texture2D backgroung[6];
int bg_index=0;
Player player[4];
Snakes snake[4];
Rectangle boardFrame = (Rectangle){47, 56, 645, 645};
Sound dice_sound, moving_sound, error_sound, ladder_up, winner, CJ;
Music bg_music;
int snakeHeads[]={81, 45, 96, 33};  
int snakeTails[]={60,23, 68, 7};
int LadderStart[]={3, 38, 27, 72};   
int LadderEnd[]={40, 78, 67, 88};
char message[100]="";
char PlayerNames[4][25]={"BLUE", "GREEN", "RED", "YELLOW"};

void ResetGameVariables(void);
void LoadAssets(void);
void InitSonds(void);
void UpdateAssets(void);
void InitializeCells(void);
void DrawPlayers(void);
void getRectangleIndex(int position, int *row, int *col);
Rectangle GetPlayerDest(int index);
void UnloadTextures(void);
void roll_n_update(void);
int CheckSnakes(Turn cur_turn);
int CheckLadders(Turn cur_turn);
