#ifndef GAMESTATE_H_                             
#define GAMESTATE_H_

typedef enum GameState
{
    INIT_N,
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
