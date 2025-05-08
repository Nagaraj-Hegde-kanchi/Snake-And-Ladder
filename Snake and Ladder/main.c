#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "raylib.h"

// **********************************************
#ifndef GAMESTATE_H_                               // CONTENT OF GameState.c FILE.
#define GAMESTATE_H_

typedef enum GameState
{
	WAITING,
	ROLLING,
    MOVING,
    OVER
} GameState;
GameState _state;
GameState GetGameState(void);   
void SetGameState(GameState);
#endif
GameState GetGameState(void)
{
	return _state;
}

void SetGameState(GameState state)
{
	_state = state;
}
// ***********************************************


void LoadAssets(void);
void UpdateAssets(void);
void InitializeCells(void);
void DrawPlayers(void);
void DrawAssets(void);
void getRectangleIndex(int position, int *row, int *col);
Rectangle GetPlayerDest(int index);
void UnloadTextures(void);
void roll_n_update();

static int i =0 ;           // this 'int i' is used in rolling animation of the dice.
int current_die_face = 1;
int dieWidth= 865;
int dieHeight=840;

typedef struct Snakes
{
    int head;
    int tail;
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
int CheckSnakes(Turn cur_turn);
int CheckLadders(Turn cur_turn);
Turn cur_turn;


typedef struct Die
{
    Texture2D texture;
    Rectangle source;
    Rectangle dest[4];
    Vector2 origin;

} Die;

Ladders ladder[4];
Die die;
Rectangle cells[10][10];
const int screenWidth = 1200;
const int screenHeight = 800;
const int cellwidth = 64;
const int cellheight = 64;
Texture2D board;
Player player[4];
Snakes snake1, snake2, snake3, snake4;
Rectangle boardFrame = (Rectangle){47, 56, 645, 645};
Sound dice_sound, moving_sound, error_sound;
int snakeHeads[]={55,63,86,43};
int snakeTails[]={44,42,53,17};
int LadderStart[]={61,23,52,13};
int LadderEnd[]={98,64,88,48};
int main(void)
{
    InitWindow(screenWidth, screenHeight, "Snake and Ladder");
    SetTargetFPS(10);
    InitAudioDevice();
    InitializeCells();
    LoadAssets();
    dice_sound = LoadSound("resources/dice-sound.ogg");
    moving_sound = LoadSound("resources/moving-sound.ogg");
    error_sound = LoadSound("resources/error-sound.ogg");
    SetGameState(WAITING);
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        UpdateAssets();
    }
    UnloadTextures();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

void UpdateAssets(void)
{

    BeginDrawing();
    ClearBackground((Color){255, 119, 210, 255}); // purple color
    DrawTexture(board, 10, 20, WHITE);
    DrawRectangle(739, 600, 100, 100, BLUE);
    DrawRectangle(739 + 117, 600, 100, 100, GREEN);
    DrawRectangle(739 + 2 * 117, 600, 100, 100, RED);
    DrawRectangle(739 + 3 * 117, 600, 100, 100, YELLOW);
    for (int i = 0; i < 4; i++)
    {
        DrawTexturePro(ladder[i].texture, ladder[i].source, ladder[i].dest, ladder[i].origin, ladder[i].rotation, WHITE);
    }
    DrawTexturePro(snake1.texture, snake1.source, snake1.dest, snake1.origin, 180, WHITE);
    DrawTexturePro(snake2.texture, snake2.source, snake2.dest, snake2.origin, 0, WHITE);
    DrawTexturePro(snake3.texture, snake3.source, snake3.dest, snake3.origin, 10, WHITE);
    DrawTexturePro(snake4.texture, snake4.source, snake4.dest, snake4.origin, -10, WHITE);
    
    if (GetGameState()==OVER)
    {
        DrawText("You Won!! Game is Over.", 750,200, 30, WHITE);
    }

    else if (GetGameState()==WAITING)
    {
        die.source=(Rectangle){(current_die_face-1)*dieWidth, 0, dieWidth, die.texture.height};
        DrawTexturePro(die.texture, die.source, die.dest[cur_turn%4], (Vector2){0, 0}, 0, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mouse_pos=GetMousePosition();
            if ((CheckCollisionPointRec(mouse_pos, die.dest[0]) && (cur_turn%4)==blue) || 
            (CheckCollisionPointRec(mouse_pos, die.dest[1]) &&(cur_turn%4)==green) ||
            (CheckCollisionPointRec(mouse_pos, die.dest[2]) && (cur_turn%4)==red) || 
            (CheckCollisionPointRec(mouse_pos, die.dest[3]) && (cur_turn%4)==yellow))
            {
                roll_n_update();
            }            
        }        
    }

    else if (GetGameState()==ROLLING)
    {
        if (i < 20)
        {
            int n = GetRandomValue(1,6);
            die.source=(Rectangle){(n-1)*dieWidth, 0, dieWidth, die.texture.height};
            DrawTexturePro(die.texture, die.source, die.dest[cur_turn%4], (Vector2){0, 0}, 0, WHITE);
            i+=1;
        }
        else{
            i=0;
            die.source=(Rectangle){(current_die_face-1)*dieWidth, 0, dieWidth, die.texture.height};
            DrawTexturePro(die.texture, die.source, die.dest[cur_turn%4], (Vector2){0, 0}, 0, WHITE);
            if (current_die_face+player[cur_turn%4].position>100){
                SetGameState(WAITING);
                cur_turn++;
                PlaySound(error_sound);
            }
            else{
                player[cur_turn%4].position+=current_die_face;
                SetGameState(MOVING);
            }            
        }

    }

    else if(GetGameState()==MOVING)
    {
        bool gotLadder=false;
        if (player[cur_turn%4].previous_position!=player[cur_turn%4].position)
        {
            player[cur_turn%4].previous_position++;
            PlaySound(moving_sound);      
        }
        else{
            if (CheckSnakes(cur_turn)>0)    //  Returns the position of tail of the snake. 0 if not bitten by snake.
            {
                player[cur_turn%4].previous_position=CheckSnakes(cur_turn);
                player[cur_turn%4].position = player[cur_turn%4].previous_position;
                PlaySound(error_sound);
            }
            else if (CheckLadders(cur_turn)>0)
            {
                player[cur_turn%4].previous_position=CheckLadders(cur_turn);        // Returns the position of head of the ladder.
                player[cur_turn%4].position = player[cur_turn%4].previous_position;
                PlaySound(error_sound);
                gotLadder=true;
            }
            if (player[cur_turn%4].position==100)
            {
                SetGameState(OVER);
                return;
            }
            
            if(current_die_face!=6 && !gotLadder)                 // another chance for getting 6 or ladder.
                cur_turn++;
            SetGameState(WAITING);
        }
    }
    DrawPlayers();
    EndDrawing();
}

void LoadAssets()
{
    board = LoadTexture("resources/board.png");

    player[0].texture = LoadTexture("resources/pieceBlue.png");
    player[0].source = (Rectangle){0, 0, player[0].texture.width, player[0].texture.height};
    player[0].position = 1;
    player[0].previous_position = 1

    player[1].texture = LoadTexture("resources/pieceGreen.png");
    player[1].source = (Rectangle){0, 0, player[1].texture.width, player[1].texture.height};
    player[1].position = 1;
    player[1].previous_position = 1;

    player[2].texture = LoadTexture("resources/pieceRed.png");
    player[2].source = (Rectangle){0, 0, player[2].texture.width, player[2].texture.height};
    player[2].position = 1;
    player[2].previous_position = 1;
    

    player[3].texture = LoadTexture("resources/pieceYellow.png");
    player[3].source = (Rectangle){0, 0, player[3].texture.width, player[3].texture.height};
    player[3].position = 1;
    player[3].previous_position =1;

    snake1.texture = LoadTexture("resources/snake1.png");
    snake1.source = (Rectangle){0, 0, snake1.texture.width, snake1.texture.height};
    snake1.dest = (Rectangle){(board.width / 2) - 20, (board.height / 2) + 15, 3.5 * snake1.texture.width, 3.5 * snake1.texture.height};
    snake1.origin = (Vector2){snake1.dest.width / 2, snake1.dest.height / 2};
    snake1.head=55;
    snake1.tail=44;

    snake2.texture = LoadTexture("resources/snake2.png");
    snake2.source = (Rectangle){0, 0, snake2.texture.width, snake2.texture.height};
    snake2.dest = (Rectangle){(board.width / 4) + 5, (board.height / 2) - 25, 2 * snake2.texture.width, 2 * snake2.texture.height};
    snake2.origin = (Vector2){snake2.dest.width / 2, snake2.dest.height / 2};
    snake2.head=63;
    snake2.tail=42;
    
    snake3.texture = LoadTexture("resources/snake3.png");
    snake3.source = (Rectangle){0, 0, snake3.texture.width, snake3.texture.height};
    snake3.dest = (Rectangle){(board.width / 2) + 100, 3*(board.height / 10)+ 25, 1.5 * snake3.texture.width, 1.5 * snake3.texture.height};
    snake3.origin = (Vector2){snake3.dest.width / 2, snake3.dest.height / 2};
    snake3.head=86;
    snake3.tail=53;

    snake4.texture = LoadTexture("resources/snake4.png");
    snake4.source = (Rectangle){0, 0, snake4.texture.width, snake4.texture.height};
    snake4.dest = (Rectangle){(board.width / 2) - 100, 7 * (board.height / 10), 1.5 * snake4.texture.width, 1.5 * snake4.texture.height};
    snake4.origin = (Vector2){snake4.dest.width / 2, snake4.dest.height / 2};
    snake4.head=43;
    snake4.tail=17;

    die.texture = LoadTexture("resources/dice_merged.png");
    die.source = (Rectangle){(current_die_face-1)*dieWidth, 0, dieWidth, die.texture.height};
    die.dest[0] = (Rectangle){739 + 5, 605, 90, 90};
    die.dest[1] = (Rectangle){739 + 117 + 5, 605, 90, 90};
    die.dest[2] = (Rectangle){5 + 739 + 2 * 117, 605, 90, 90};
    die.dest[3] = (Rectangle){5 + 739 + 3 * 117, 605, 90, 90};

    ladder[0].texture=LoadTexture("resources/ladder1.png");
    ladder[0].origin= (Vector2){0,0};
    ladder[0].source=(Rectangle){0, 0, ladder[0].texture.width, ladder[0].texture.height};
    ladder[0].dest=(Rectangle){510, 410, ladder[0].texture.width * 3, 4 * ladder[0].texture.height};
    ladder[0].rotation=0;

    ladder[1].texture=LoadTexture("resources/ladder2.png");
    ladder[1].source=(Rectangle){0, 0, ladder[1].texture.width, ladder[1].texture.height};
    ladder[1].dest=(Rectangle){150, 180, ladder[1].texture.width * 3, 4 * ladder[1].texture.height};
    ladder[1].origin= (Vector2){ladder[1].dest.width/2 ,ladder[1].dest.height/2 };
    ladder[1].rotation=30;

    ladder[2].texture=LoadTexture("resources/ladder3.png");
    ladder[2].source=(Rectangle){0, 0, ladder[2].texture.width, ladder[2].texture.height};
    ladder[2].dest=(Rectangle){560, 260, ladder[2].texture.width * 1.2, 4 * ladder[2].texture.height};
    ladder[2].origin= (Vector2){ladder[2].dest.width/2 ,ladder[2].dest.height/2 };
    ladder[2].rotation=-25;

    ladder[3].texture=LoadTexture("resources/ladder4.png");
    ladder[3].source=(Rectangle){0, 0, ladder[3].texture.width, ladder[3].texture.height};
    ladder[3].dest=(Rectangle){260, 360, ladder[3].texture.width * 1.2, 2 * ladder[3].texture.height};
    ladder[3].origin= (Vector2){ladder[3].dest.width/2 ,ladder[2].dest.height/2 };
    ladder[3].rotation=20;

}

void InitializeCells(void)
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            cells[i][j] = (Rectangle){50 + (cellwidth * j), 60 + (cellheight * i), cellwidth, cellheight};
        }
    }
}

void DrawPlayers(void)
{
    getRectangleIndex(player[0].previous_position, &player[0].row, &player[0].column);
    getRectangleIndex(player[1].previous_position, &player[1].row, &player[1].column);
    getRectangleIndex(player[2].previous_position, &player[2].row, &player[2].column);
    getRectangleIndex(player[3].previous_position, &player[3].row, &player[3].column);

    DrawTexturePro(player[0].texture, player[0].source, GetPlayerDest(0), (Vector2){0, 0}, 0, WHITE); 
    DrawTexturePro(player[1].texture, player[1].source, GetPlayerDest(1), (Vector2){0, 0}, 0, WHITE);
    DrawTexturePro(player[2].texture, player[2].source, GetPlayerDest(2), (Vector2){0, 0}, 0, WHITE);
    DrawTexturePro(player[3].texture, player[3].source, GetPlayerDest(3), (Vector2){0, 0}, 0, WHITE);
    return;
}

void getRectangleIndex(int position, int *row, int *col)
{
    if (position == 100)
    {
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


Rectangle GetPlayerDest(int index){
    int occupied = 0;
    for (int j = 0; j < 4; j++)
    {
        if (player[j].previous_position==player[index].previous_position)
        {
            occupied++;
        }
    }
    if (occupied==1)
    {
        return cells[player[index].row][player[index].column];
    }
    
    else if(occupied==2){
        int xpos=((cells[player[index].row][player[index].column]).x) + index*10;
        int ypos=((cells[player[index].row][player[index].column]).y);
        int width=((cells[player[index].row][player[index].column]).width - 10);
        int height=((cells[player[index].row][player[index].column]).height -10);
        return (Rectangle){xpos, ypos, width, height};
    }
    else
    {
        int xpos=((cells[player[index].row][player[index].column]).x)+2*occupied;
        int ypos=((cells[player[index].row][player[index].column]).y)+2*occupied;
        int width=((cells[player[index].row][player[index].column]).width);
        int height=((cells[player[index].row][player[index].column]).height);
        return (Rectangle){xpos, ypos, width, height};
    }
    
}

void UnloadTextures(){
    UnloadTexture(die.texture);
    UnloadTexture(snake1.texture);
    UnloadTexture(snake2.texture);
    UnloadTexture(snake3.texture);
    UnloadTexture(snake4.texture);
    for (int i = 0; i < 4; i++)
    {
        UnloadTexture(player[i].texture);
    }
    UnloadTexture(board);    
}

void roll_n_update(){
    current_die_face=GetRandomValue(1,6);
    PlaySound(dice_sound);
    SetGameState(ROLLING);
}

int CheckSnakes(Turn cur_turn){
    for (int i = 0; i < 4; i++)
    {
        if (snakeHeads[i]==player[cur_turn%4].position)
        {
            return snakeTails[i];
        }       
    }
    return 0;
}

int CheckLadders(Turn cur_turn){
    for (int i = 0; i < 4; i++)
    {
        if (LadderStart[i]==player[cur_turn%4].position)
        {
            return LadderEnd[i];
        }
    }
    return 0;
}
