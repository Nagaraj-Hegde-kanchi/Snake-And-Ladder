#ifndef STD_H_
    #define STD_H_
    #include <stdio.h>
#endif
#include <string.h>
#ifndef RAYLIB_H_
    #define RAYLIB_H_
    #include "raylib.h"
#endif
#include "GameState.c"
#include "Timer.c"
#include "SnL_Main.h"

int main(void)
{
    InitWindow(screenWidth, screenHeight, "Snake and Ladder");
    SetTargetFPS(30);
    InitAudioDevice();
    InitializeCells();
    LoadAssets();
    InitSonds();
    PlayMusicStream(bg_music);
    SetGameState(INIT_N);
    strcpy(message, "Press B to change background\nClick on the die to Roll");
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        UpdateMusicStream(bg_music);
        UpdateAssets();
    }
    StopMusicStream(bg_music);
    UnloadTextures();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

void UpdateAssets(void)
{
    BeginDrawing();
    ClearBackground((Color){255, 119, 210, 255}); // purple color
        DrawTexturePro(backgroung[bg_index%6],
                    (Rectangle){0,0,backgroung[bg_index%6].width,backgroung[bg_index%6].height}, 
                    (Rectangle){0 , 0,screenWidth, screenHeight},
                    (Vector2){0,0}, 
                    0, WHITE);                          // Drawing Background image
    DrawTexture(board, 10, 20, WHITE);
    DrawRectangle(789, 450, 120, 120, BLUE);            // these are the rectangle where the die will appear
    DrawRectangle(989, 450, 120, 120, GREEN);
    DrawRectangle(789 , 600, 120, 120, RED);
    DrawRectangle(989, 600, 120, 120, YELLOW);
    for (int i = 0; i < 4; i++)
    {
        DrawTexturePro(ladder[i].texture, ladder[i].source, ladder[i].dest, ladder[i].origin, ladder[i].rotation, WHITE);
    }

    DrawTexturePro(snake[0].texture, snake[0].source, snake[0].dest, snake[0].origin, 155, WHITE);
    DrawTexturePro(snake[1].texture, snake[1].source, snake[1].dest, snake[1].origin, 0, WHITE);
    DrawTexturePro(snake[2].texture, snake[2].source, snake[2].dest, snake[2].origin, -10, WHITE);
    DrawTexturePro(snake[3].texture, snake[3].source, snake[3].dest, snake[3].origin, 0, WHITE);
    DrawFPS(1120, 10);

    if (GetGameState()==INIT_N)             // get the number of players
    {
        static bool temp=true;      // for cursor animation
        if (temp)
        {
            DrawText("Number Of Players: | " , 750,150, 30, RED);
            temp=(!temp);
        }
        else{
            DrawText("Number Of Players:  " , 750,150, 30, RED);
            temp=(!temp);
        }
        if (IsKeyPressed(KEY_B))
        {
            bg_index=(bg_index+1)%6;
        }
        if (IsKeyPressed(KEY_ONE))
        {
            n=1;  SetGameState(WAITING);   
        }
        else if(IsKeyPressed(KEY_TWO))
        {
            n=2;  SetGameState(WAITING);
        }
        else if(IsKeyPressed(KEY_THREE))
        {
            n=3;  SetGameState(WAITING);
        }
        else if (IsKeyPressed(KEY_FOUR))
        {
            n=4; SetGameState(WAITING);
        }
        
    }
    
    else if (GetGameState()==OVER)
    {
        PauseMusicStream(bg_music);
        strcpy(message, "");
        sprintf(message, "Player %s won the game\nCongratulations!\nPress R to reset", PlayerNames[cur_turn%n]);
        DrawText( message, 750,200, 28, WHITE);
        if (IsKeyPressed(KEY_R))
        {
            ResetGameVariables();
            PlaySound(CJ);
            strcpy(message, " Better Luck this time!!");
            SetGameState(INIT_N);
        }
    }

    else if (GetGameState()==WAITING)
    {
        die.source=(Rectangle){(current_die_face-1)*dieWidth, 0, dieWidth, die.texture.height};
        DrawTexturePro(die.texture, die.source, die.dest[cur_turn%n], (Vector2){0, 0}, 0, WHITE);
        DrawText( message, 750, 320, 28, WHITE);

        if (IsKeyPressed(KEY_S))
        {
            Gspeedptr++;
            sprintf(message, "Game Speed = %.2f", (1.0 - GameSpeed[Gspeedptr%6])*100.0 );
            DrawText(message, 750, 320, 28, WHITE);
        }

        if (IsKeyPressed(KEY_W))
        {
            Gspeedptr = (Gspeedptr+5)%6;
            sprintf(message, "Game Speed = %.1f", (1.0 - GameSpeed[Gspeedptr%6])*100.0 );
            DrawText(message, 750, 320, 28, WHITE);
        }
        
        if (IsKeyPressed(KEY_B))
        {
            bg_index=(bg_index+1)%6;
        }

        if (IsKeyPressed(KEY_C) && IsKeyPressed(KEY_H))
        {
            for (int k = 0; k < n; k++){
                player[k].position = 99;
                player[k].previous_position =99;
            }
            strcpy(message, "Cheat Activated");
        }
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mouse_pos=GetMousePosition();
            if ((CheckCollisionPointRec(mouse_pos, die.dest[0]) && (cur_turn%n)==blue) || 
            (CheckCollisionPointRec(mouse_pos, die.dest[1]) &&(cur_turn%n)==green) ||
            (CheckCollisionPointRec(mouse_pos, die.dest[2]) && (cur_turn%n)==red) || 
            (CheckCollisionPointRec(mouse_pos, die.dest[3]) && (cur_turn%n)==yellow))
            {
                roll_n_update();
            }            
        }        
    }

    else if (GetGameState()==ROLLING)
    {
        DrawText( "Rolling......", 750,320, 30, WHITE);
        if (i < 40)                 // Animating Die
        {
            int temp = GetRandomValue(1,6);
            die.source=(Rectangle){(temp-1)*dieWidth, 0, dieWidth, die.texture.height};
            DrawTexturePro(die.texture, die.source, die.dest[cur_turn%n], (Vector2){0, 0}, 0, WHITE);
            i+=1;
        }
        else{                      // End of Animation
            strcpy(message, PlayerNames[cur_turn%n]);
            strcat(message, " Rolled ");
            char str[10];
            sprintf(str, "%d", current_die_face);
            strcat(message, str);
            DrawText( message, 750,320, 28, WHITE);
            
            i=0;
            die.source=(Rectangle){(current_die_face-1)*dieWidth, 0, dieWidth, die.texture.height};
            DrawTexturePro(die.texture, die.source, die.dest[cur_turn%n], (Vector2){0, 0}, 0, WHITE);
            if (current_die_face+player[cur_turn%n].position>100){
                SetGameState(WAITING);
                cur_turn++;
                PlaySound(error_sound);
            }
            else{
                player[cur_turn%n].position+=current_die_face;
                SetGameState(MOVING);
            }            
        }

    }

    else if(GetGameState()==MOVING)
    {
        die.source=(Rectangle){(current_die_face-1)*dieWidth, 0, dieWidth, die.texture.height};
        DrawTexturePro(die.texture, die.source, die.dest[cur_turn%n], (Vector2){0, 0}, 0, WHITE);
        DrawText( message, 750, 320, 28, WHITE);
        bool gotLadder=false;
        static Timer PieceTimer;
        static bool timerInitialized = false;
        if (!timerInitialized) {
            StartTimer(&PieceTimer, GameSpeed[Gspeedptr%6]);
            timerInitialized = true;
        }

        if (player[cur_turn%n].previous_position!=player[cur_turn%n].position)
        {
            UpdateTimer(&PieceTimer);
            if (IsTimerDone(&PieceTimer))
            {
                player[cur_turn%n].previous_position++;
                PlaySound(moving_sound);
                StartTimer(&PieceTimer, GameSpeed[Gspeedptr%6]); 
            }     
        }
        else{
            if (CheckSnakes(cur_turn%n)>0)    //  Returns the position of tail of the snake. 0 if not bitten by snake.
            {
                player[cur_turn%n].previous_position=CheckSnakes(cur_turn);
                player[cur_turn%n].position = player[cur_turn%n].previous_position;
                PlaySound(error_sound);
            }
            else if (CheckLadders(cur_turn)>0)
            {
                player[cur_turn%n].previous_position=CheckLadders(cur_turn);        // Returns the position of head of the ladder.
                player[cur_turn%n].position = player[cur_turn%n].previous_position;
                PlaySound(ladder_up);
                gotLadder=true;
            }
            if (player[cur_turn%n].previous_position==100)
            {
                PlaySound(winner);
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
    player[1].texture = LoadTexture("resources/pieceGreen.png");
    player[2].texture = LoadTexture("resources/pieceRed.png");
    player[3].texture = LoadTexture("resources/pieceYellow.png");

    for (int k = 0; k < 4; k++){
        player[k].source = (Rectangle){0, 0, player[k].texture.width, player[k].texture.height};
        player[k].position = 1;
        player[k].previous_position =1;
    }

    snake[0].texture = LoadTexture("resources/snake1.png");
    snake[0].source = (Rectangle){0, 0, snake[0].texture.width, snake[0].texture.height};
    snake[0].dest = (Rectangle){ 510, 568, 4.2* snake[0].texture.width, 4.2*snake[0].texture.height};
    snake[0].origin = (Vector2){snake[0].dest.width / 2, snake[0].dest.height / 2};

    snake[1].texture = LoadTexture("resources/snake2.png");
    snake[1].source = (Rectangle){0, 0, snake[1].texture.width, snake[1].texture.height};
    snake[1].dest = (Rectangle){281 , 464, 3 * snake[1].texture.width, 3 * snake[1].texture.height};
    snake[1].origin = (Vector2){snake[1].dest.width / 2, snake[1].dest.height / 2};

    snake[2].texture = LoadTexture("resources/snake3.png");
    snake[2].source = (Rectangle){0, 0, snake[2].texture.width, snake[2].texture.height};
    snake[2].dest = (Rectangle){434, 175, 1.5*snake[2].texture.width , 1.5*snake[2].texture.height};
    snake[2].origin = (Vector2){snake[2].dest.width / 2, snake[2].dest.height / 2};


    snake[3].texture = LoadTexture("resources/snake4.png");
    snake[3].source = (Rectangle){0, 0, snake[3].texture.width, snake[3].texture.height};
    snake[3].dest = (Rectangle){ 80 , 250, 1.5 * snake[3].texture.width, 1.5 * snake[3].texture.height};
    snake[3].origin = (Vector2){snake[3].dest.width / 2, snake[3].dest.height / 2};

    die.texture = LoadTexture("resources/dice_merged.png");
    die.source = (Rectangle){(current_die_face-1)*dieWidth, 0, dieWidth, die.texture.height};
    die.dest[0] = (Rectangle){799, 460, 100, 100};
    die.dest[1] = (Rectangle){999, 460, 100, 100};
    die.dest[2] = (Rectangle){799 , 610, 100, 100};
    die.dest[3] = (Rectangle){999, 610, 100, 100};

    ladder[0].texture=LoadTexture("resources/ladder1.png");
    ladder[0].origin= (Vector2){0,0};
    ladder[0].source=(Rectangle){0, 0, ladder[0].texture.width, ladder[0].texture.height};
    ladder[0].dest=(Rectangle){85, 491, ladder[0].texture.width*3, 4.5*ladder[0].texture.height};
    ladder[0].rotation=-30;

    ladder[1].texture=LoadTexture("resources/ladder2.png");
    ladder[1].source=(Rectangle){0, 0, ladder[1].texture.width, ladder[1].texture.height};
    ladder[1].dest=(Rectangle){464, 408, ladder[1].texture.width * 3, 4.5*ladder[1].texture.height};
    ladder[1].origin= (Vector2){ladder[1].dest.width/2 ,ladder[1].dest.height/2 };
    ladder[1].rotation=0;

    ladder[2].texture=LoadTexture("resources/ladder3.png");
    ladder[2].source=(Rectangle){0, 0, ladder[2].texture.width, ladder[2].texture.height};
    ladder[2].dest=(Rectangle){560, 186, ladder[2].texture.width * 1.2, 2* ladder[2].texture.height};
    ladder[2].origin= (Vector2){ladder[2].dest.width/2 ,ladder[2].dest.height/2 };
    ladder[2].rotation=-40;

    ladder[3].texture=LoadTexture("resources/ladder4.png");
    ladder[3].source=(Rectangle){0, 0, ladder[3].texture.width, ladder[3].texture.height};
    ladder[3].dest=(Rectangle){ 208 , 250, ladder[3].texture.width * 1.2, 2 * ladder[3].texture.height};
    ladder[3].origin= (Vector2){ladder[3].dest.width/2 ,ladder[2].dest.height/2 };
    ladder[3].rotation= 0 ;

    backgroung[0]=LoadTexture("resources/background1.png");
    backgroung[1]=LoadTexture("resources/background2.png");
    backgroung[2]=LoadTexture("resources/background3.png");
    backgroung[3]=LoadTexture("resources/background4.png");
    backgroung[4]=LoadTexture("resources/background5.png");
    backgroung[5]=LoadTexture("resources/background6.png");
}

void InitSonds(void){
    bg_music=LoadMusicStream("resources/bg-music-1.mp3");
    dice_sound = LoadSound("resources/dice-sound.ogg");
    moving_sound = LoadSound("resources/moving-sound.ogg");
    error_sound = LoadSound("resources/error-sound.ogg");
    ladder_up = LoadSound("resources/ladder-up.ogg");
    winner = LoadSound("resources/winner.ogg");
    CJ = LoadSound("resources/Voicy_Ah-shit_-here-we-go-again.ogg");
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
    for (int j = 0; j < n; j++){
        getRectangleIndex(player[j].previous_position, &player[j].row, &player[j].column);
    }

    for (int j = 0; j < n; j++){
        DrawTexturePro(player[j].texture, player[j].source, GetPlayerDest(j), (Vector2){0, 0}, 0, WHITE); 
    }
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
    for (int j = 0; j < n; j++)
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
    
    else{
        int xpos, ypos, width, height;
        width=((cells[player[index].row][player[index].column]).width - 10);
        height=((cells[player[index].row][player[index].column]).height -10);
        if (index==0)
        {
            xpos=((cells[player[index].row][player[index].column]).x);
            ypos=((cells[player[index].row][player[index].column]).y);
        }
        else if (index==1)
        {
            xpos=((cells[player[index].row][player[index].column]).x) + 10;
            ypos=((cells[player[index].row][player[index].column]).y);
        }
        else if (index==2)
        {
            xpos=((cells[player[index].row][player[index].column]).x);
            ypos=((cells[player[index].row][player[index].column]).y) + 10;
        }
        else
        {
            xpos=((cells[player[index].row][player[index].column]).x) + 10;
            ypos=((cells[player[index].row][player[index].column]).y) + 10;
        }
        return (Rectangle){xpos, ypos, width, height};
    }
    
}

void UnloadTextures(void){

    UnloadTexture(die.texture);
    for (int i = 0; i < 4; i++)
    {
        UnloadTexture(snake[i].texture);
        UnloadTexture(player[i].texture);
    }
    UnloadTexture(board);
    UnloadSound(dice_sound);    
    UnloadSound(moving_sound);    
    UnloadSound(error_sound);    
    UnloadSound(ladder_up);    
    UnloadSound(winner);    
}

void roll_n_update(void){
    current_die_face=GetRandomValue(1,6);
    PlaySound(dice_sound);
    SetGameState(ROLLING);
}

int CheckSnakes(Turn cur_turn){
    for (int i = 0; i < 4; i++)
    {
        if (snakeHeads[i]==player[cur_turn%n].position)
        {
            return snakeTails[i];
        }       
    }
    return 0;
}

int CheckLadders(Turn cur_turn){
    for (int i = 0; i < 4; i++)
    {
        if (LadderStart[i]==player[cur_turn%n].position)
        {
            return LadderEnd[i];
        }
    }
    return 0;
}

void ResetGameVariables(void){
    ResumeMusicStream(bg_music);
    cur_turn=0;
    current_die_face=1;
    for (int k = 0; k < n; k++){
        player[k].position = 1;
        player[k].previous_position =1;
    }
}
