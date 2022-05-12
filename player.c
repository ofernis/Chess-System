#include "map.h"
#include "chessSystem.h"
#include "player.h"
#include <stdlib.h>

struct player_t {
    int id;
    int wins;
    int loses;
    int draws;
    int total_play_time;
};

struct rank_t {
        double level;
        int id;
}; 

MapDataElement copyDataPlayer(MapDataElement element)
{
    if (element == NULL)
    {
        return NULL;
    }

    Player new_player = playerCopy((Player)element);
    if (new_player == NULL)
    {
        return NULL;
    }
    return new_player;
}

MapKeyElement copyKeyPlayerID(MapKeyElement element)
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

MapKeyElement copyKeyRank(MapKeyElement element)
{
    if (element == NULL)
    {
        return NULL;
    }

    Rank new_rank = malloc(sizeof(*new_rank));
    if (new_rank == NULL){
        return NULL;
    }
    new_rank->level = ((Rank) element)->level;
    new_rank->id = ((Rank) element)->id;
    return new_rank;
}

void freeDataPlayer(MapDataElement element)
{
    playerDestroy((Player)element);
}

void freeKeyPlayerID(MapKeyElement element)
{
    free(element);
}

void freeKeyRank(MapKeyElement element)
{
    free(element);
}

int compareKeyPlayerID(MapKeyElement element1, MapKeyElement element2)
{
    return *(int*) element1 - *(int*) element2;
}

//gets num sign - positive or negative
int sign(double num)
{
    return (num < 0) - (num > 0);
}

int compareKeyRank(MapKeyElement element1, MapKeyElement element2)
{
    Rank rank_element1 = (Rank) element1;
    Rank rank_element2 = (Rank) element2;
    double level_difference = rank_element1->level - rank_element2->level;

    if (level_difference != 0)
    {
        return sign(level_difference); //use sign function because of double to int cast (for example 0.1 would be round to 0 and function will return 0)
    }
    else
    {
        return rank_element1->id - rank_element2->id;
    }
}

Map playerMapFactory()
{
    Map player_map = mapCreate(copyDataPlayer, copyKeyPlayerID, freeDataPlayer, freeKeyPlayerID, compareKeyPlayerID);
    if (player_map == NULL)
    {
        return NULL;
    }
    return player_map;
}

Map playersRankMapFactory()
{
    Map tournament_map = mapCreate(copyDataPlayer, copyKeyRank, freeDataPlayer, freeKeyRank, compareKeyRank);
    if (tournament_map == NULL)
    {
        return NULL;
    }
    return tournament_map;
}

Player playerCreate(int id)
{
    Player new_player = malloc(sizeof(*new_player));
    if (new_player == NULL)
    {
        return NULL;
    }
    
    new_player->id = id;
    new_player->wins = 0;
    new_player->loses = 0;
    new_player->draws = 0;
    new_player->total_play_time = 0;

    return new_player;
}

Player playerCopy(Player player)
{
    if (player == NULL)
    {
        return NULL;
    }

    Player player_cpy = playerCreate(player->id);
    if (player_cpy == NULL)
    {
        return NULL;
    }

    player_cpy->wins = player->wins;
    player_cpy->loses = player->loses;
    player_cpy->draws = player->draws;
    player_cpy->total_play_time = player->total_play_time;
    return player_cpy;
}
void playerDestroy(Player player)
{
    if (player == NULL)
    {
        return;
    }
    free(player);
}

Rank playerRankCreate(double level, int id)
{
    Rank new_rank = malloc(sizeof(*new_rank));
    if (new_rank == NULL)
    {
        return NULL;
    }

    new_rank->level = level;
    new_rank->id = id;
    return new_rank;
}

void playerRankDestroy(Rank rank)
{
    if (rank == NULL)
    {
        return;
    }
    free(rank);
}

int playerRankGetID(Rank rank)
{
    return rank->id;
}

double playerRankGetLevel(Rank rank)
{
    return rank->level;
}

int playerGetID(Player player)
{
    if (player == NULL)
    {
        return PLAYER_NOT_EXIST;
    }
    return player->id;
}

void playerSetID(Player player, int id)
{
    if (player == NULL)
    {
        return;
    }
    player->id = id;
}

int playerGetWins(Player player)
{
    if (player == NULL)
    {
        return PLAYER_NOT_EXIST;
    }
    return player->wins;
}

int playerGetLoses(Player player)
{
    if (player == NULL)
    {
        return PLAYER_NOT_EXIST;
    }
    return player->loses;
}

int playerGetDraws(Player player)
{
    if (player == NULL)
    {
        return PLAYER_NOT_EXIST;
    }
    return player->draws;
}

int playerGetTotalPlayTime(Player player)
{
    if (player == NULL)
    {
        return PLAYER_NOT_EXIST;
    }
    return player->total_play_time;
}

int playerGetScore(Player player)
{
    int wins = player->wins;
    int draws = player->draws;
    return (2 * wins + draws);
}

void playersAddScore(Player players[], Winner score)
{
    if (players[FIRST_PLAYER] == NULL || players[SECOND_PLAYER] == NULL)
    {
        return;
    }
    else if (score == DRAW)
    {
        players[FIRST_PLAYER]->draws++;
        players[SECOND_PLAYER]->draws++;
    }
    else
    {
        players[score]->wins++;
        players[(score + 1) % 2]->loses++;
    }
}

double playerGetLevel(Player player)
{
    double wins = playerGetWins(player);
    double loses = playerGetLoses(player);
    double draws = playerGetDraws(player);
    double n = wins + loses + draws; // n is the total number of games played by the player
    if (n == 0)
    {
        return n;
    }
    return (6 * wins - 10 * loses + 2 *draws) / n;
}

void playersRemoveScore(Player players[], Winner score)
{
    if (players[FIRST_PLAYER] == NULL || players[SECOND_PLAYER] == NULL)
    {
        int not_null_player = (players[FIRST_PLAYER] == NULL) ? SECOND_PLAYER : FIRST_PLAYER;
        players[not_null_player]->wins--;
    }
    else if (score == DRAW)
    {
        players[FIRST_PLAYER]->draws--;
        players[SECOND_PLAYER]->draws--;
    }
    else
    {
        players[score]->wins--;
        players[(score + 1) % 2]->loses--;
    }
}

void playersAddPlayTime(Player players[], int play_time)
{
    if (players[FIRST_PLAYER] == NULL || players[SECOND_PLAYER] == NULL)
    {
        int not_null_player = (players[FIRST_PLAYER] == NULL) ? SECOND_PLAYER : FIRST_PLAYER;
        players[not_null_player]->total_play_time = players[not_null_player]->total_play_time + play_time;
    }
    else
    {
        players[FIRST_PLAYER]->total_play_time = players[FIRST_PLAYER]->total_play_time + play_time;
        players[SECOND_PLAYER]->total_play_time = players[SECOND_PLAYER]->total_play_time + play_time;
    }    
}

void playersRemovePlayTime(Player players[], int play_time)
{
    if (players[FIRST_PLAYER] == NULL || players[SECOND_PLAYER] == NULL)
    {
        int not_null_player = (players[FIRST_PLAYER] == NULL) ? SECOND_PLAYER : FIRST_PLAYER;
        players[not_null_player]->total_play_time = players[not_null_player]->total_play_time - play_time;
    }
    else
    {
        players[FIRST_PLAYER]->total_play_time = players[FIRST_PLAYER]->total_play_time - play_time;
        players[SECOND_PLAYER]->total_play_time = players[SECOND_PLAYER]->total_play_time - play_time;
    }
}