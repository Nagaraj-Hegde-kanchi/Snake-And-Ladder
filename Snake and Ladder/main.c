#include <stdio.h>
#include "raylib.h"
void LoadAssets(void);
void UpdateAssets(void);
void InitializeCells(void);
void DrawPlayers(void);
void DrawAssets(void);
void getRectangleIndex(int position, int *row, int *col);

typedef struct Snakes
{
    Texture2D texture;
    Rectangle source;
    Rectangle dest;
    Vector2 origin;
}Snakes;

typedef struct Player
{
    Texture2D texture;
    Rectangle source;
    int position;
    int row;
    int column;
}Player;

typedef struct Die
{
    Texture2D texture;
    Rectangle source;
    Rectangle dest;
    Vector2 origin;
}Die;


Die die[6];
Rectangle cells[10][10];
const int screenWidth = 1200;
const int screenHeight = 800;
const int cellwidth=64;
const int cellheight=64;
Texture2D board;
Player player_blue;
Snakes snake1, snake2, snake3, snake4;
Rectangle boardFrame=(Rectangle){47, 56 , 645, 645};

int main(void)
{
    InitWindow(screenWidth, screenHeight, "Snake and Ladder");
    SetTargetFPS(60); 
    InitializeCells();   
    LoadAssets();
    DrawAssets();                                 
               
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateAssets();        
    }
    CloseWindow();      
    return 0;
}

void UpdateAssets(void){
    
    BeginDrawing();
        ClearBackground((Color){ 255, 119, 210, 255 });   // purple color
        DrawTexture(board, 10, 20, WHITE);
        DrawRectangle(739, 600,100, 100, BLUE);
        DrawRectangle(739+117, 600,100, 100, GREEN);
        DrawRectangle(739+2*117, 600,100, 100, RED);
        DrawRectangle(739+3*117, 600,100, 100, YELLOW);
        DrawTexturePro(snake1.texture, snake1.source, snake1.dest, snake1.origin, 180, WHITE);  
        DrawTexturePro(snake2.texture, snake2.source, snake2.dest, snake2.origin, 0, WHITE);  
        DrawTexturePro(snake3.texture, snake3.source, snake3.dest, snake3.origin, 10, WHITE);  
        DrawTexturePro(snake4.texture, snake4.source, snake4.dest, snake4.origin, -10, WHITE);
        DrawPlayers();
        DrawTexturePro(die[0].texture, die[0].source, die[0].dest, die[0].origin, 0, WHITE);             
    EndDrawing();
}

void LoadAssets(){
    board = LoadTexture("resources/board.png"); 
    player_blue.texture=LoadTexture("resources/pieceBlue.png");
    player_blue.source=(Rectangle){0, 0, player_blue.texture.width, player_blue.texture.height};
    player_blue.position=0;

    snake1.texture=LoadTexture("resources/snake1.png");
    snake1.source=(Rectangle){0, 0, snake1.texture.width, snake1.texture.height};
    snake1.dest=(Rectangle){(board.width/2)+5, (board.height/2)+5, 3.5*snake1.texture.width, 3.5*snake1.texture.height};
    snake1.origin=(Vector2){snake1.dest.width/2, snake1.dest.height/2};
    
    snake2.texture=LoadTexture("resources/snake2.png");
    snake2.source=(Rectangle){0, 0, snake2.texture.width, snake2.texture.height};
    snake2.dest=(Rectangle){(board.width/4)+5, (board.height/2)-25, 2*snake2.texture.width, 2*snake2.texture.height};
    snake2.origin=(Vector2){snake2.dest.width/2, snake2.dest.height/2};

    snake3.texture=LoadTexture("resources/snake3.png");
    snake3.source=(Rectangle){0, 0, snake3.texture.width, snake3.texture.height};
    snake3.dest=(Rectangle){(board.width/2)+100, 3*(board.height/10), 1.5*snake3.texture.width, 1.5*snake3.texture.height};
    snake3.origin=(Vector2){snake3.dest.width/2, snake3.dest.height/2};

    snake4.texture=LoadTexture("resources/snake4.png");
    snake4.source=(Rectangle){0, 0, snake4.texture.width, snake4.texture.height};
    snake4.dest=(Rectangle){(board.width/2)-100, 7*(board.height/10), 1.5*snake4.texture.width, 1.5*snake4.texture.height};
    snake4.origin=(Vector2){snake4.dest.width/2, snake4.dest.height/2};

    die[0].texture=LoadTexture("resources/dice1.png");
    die[0].source=(Rectangle){0, 0, die[0].texture.width, die[0].texture.height};
    die[0].dest=(Rectangle){739+5, 600+5, 90, 90};
}

void InitializeCells(void){
    for(int i=0; i<10; i++){
        for(int j=0; j<10; j++){
            cells[i][j]=(Rectangle){50+(cellwidth*j), 60+(cellheight*i), cellwidth, cellheight};
        }
    }
}

void DrawPlayers(void){
    getRectangleIndex(player_blue.position, &player_blue.row, &player_blue.column);
    DrawTexturePro(player_blue.texture, player_blue.source, cells[player_blue.row][player_blue.column], (Vector2){0,0}, 0, WHITE);
    return;
}


void DrawAssets(void){
    BeginDrawing();
        ClearBackground((Color){ 255, 119, 210, 255 });   // purple color
        DrawTexture(board, 10, 20, WHITE);
        DrawTexturePro(snake1.texture, snake1.source, snake1.dest, snake1.origin, 180, WHITE);  
        DrawTexturePro(snake2.texture, snake2.source, snake2.dest, snake2.origin, 0, WHITE);  
        DrawTexturePro(snake3.texture, snake3.source, snake3.dest, snake3.origin, 10, WHITE);  
        DrawTexturePro(snake4.texture, snake4.source, snake4.dest, snake4.origin, -10, WHITE);
        DrawPlayers();
    EndDrawing();
    return;
}



void getRectangleIndex(int position, int *row, int *col)
{
    if (position==100){
        *row = 0;
        *col = 0;
        return;
    }
    if (position < 1 || position > 100)
    {
        *row = -1;
        *col = -1;
        return;
    }

    int r = (position - 1) / 10;
    int c = (position - 1) % 10;

    *row = 9 - r;
    if (r % 2 == 0)
    {

        *col = c;
    }
    else
    {
        *col = 9 - c;
    }
}