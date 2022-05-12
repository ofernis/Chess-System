#include "game.h"
#include "player.h"
#include <stdlib.h>


struct game_t {
    Winner winner;
    int play_time;
    int first_player;
    int second_player;
};

MapDataElement copyDataGame(MapDataElement element)
{
    if (element == NULL)
    {
        return NULL;
    }
    Game new_game = gameCopy((Game) element);
    if (new_game == NULL)
    {
        return NULL;
    }
    return new_game;
}

MapKeyElement copyKeyGameID(MapKeyElement element)
{
    if (element == NULL)
    {
        return NULL;
    }

    int* new_int = malloc(sizeof(*new_int));
    if (new_int == NULL){
        return NULL;
    }
    *new_int = *(int*) element;
    return new_int;
}

void freeDataGame(MapDataElement element)
{
    gameDestroy((Game) element);
}

void freeKeyGameID(MapKeyElement element)
{
    free(element);
}

int compareKeyGameID(MapKeyElement element1, MapKeyElement element2)
{
    return *(int*) element1 - *(int*) element2;
}

Map gameMapFactory()
{
    Map game_map = mapCreate(copyDataGame, copyKeyGameID, freeDataGame, freeKeyGameID, compareKeyGameID);
    if (game_map == NULL)
    {
        return NULL;
    }

    return game_map;
}

Game gameCreate(Winner winner, int play_time, int first_player, int second_player)
{
    Game new_game = malloc(sizeof(*new_game));
    if(new_game == NULL)
    {
        return NULL;
    }

    new_game->winner = winner;
    new_game->play_time = play_time;
    new_game->first_player = first_player;
    new_game->second_player = second_player;

    return new_game;
}

Game gameCopy(Game game)
{
    if (game == NULL)
    {
        return NULL;
    }

    Game game_cpy = gameCreate(game->winner, game->play_time, game->first_player, game->second_player);
    if (game_cpy == NULL)
    {
        return NULL;
    }
    return game_cpy;
}

void gameDestroy(Game game)
{
    if (game == NULL)
    {
        return;
    }

    free(game);
}

int gameGetWinner(Game game)
{
    return game->winner;
}

bool isPlayerInCurrentGame(Game game, int player_id)
{
    return gameGetFirstPlayer(game) == player_id ||  gameGetSecondPlayer(game) == player_id;
}

int gameGetOpponent(Game game, int opponent_id)
{
    if (game == NULL)
    {
        return GAME_NOT_EXIST;
    }

    if (game->first_player == opponent_id)
    {
        return game->second_player;
    }
    return game->first_player;
}

void gameSetWinner(Game game, int winner_id)
{
    if (game == NULL)
    {
        return;
    }

    if (game->first_player == winner_id)
    {
        game->winner = FIRST_PLAYER;
    }

    else
    {
        game->winner = SECOND_PLAYER;
    }
}

int gameGetPlayTime(Game game)
{
    if (game == NULL)
    {
        return GAME_NOT_EXIST;
    }
    return game->play_time;
}

int gameGetFirstPlayer(Game game)
{
    if (game == NULL)
    {
        return GAME_NOT_EXIST;
    }
    return game->first_player;
}

int gameGetSecondPlayer(Game game)
{
    if (game == NULL)
    {
        return GAME_NOT_EXIST;
    }
    return game->second_player;
}

void gameUpdateDeletedPlayerID(Game game, int player_id)
{
    if (game == NULL)
    {
        return;
    }
    if (game->first_player == player_id)
    {
        game->first_player = (-1) * player_id;
    }
    else
    { 
        game->second_player = (-1) * player_id;    
    }
}
