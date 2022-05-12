#include <stdlib.h>
#include <stdbool.h>
#include "tournament.h"
#include "game.h"
#include "player.h"
#include "map.h"

struct tournament_t {
    Map games;
    Map players;
    int winner;
    int max_games_per_player;
    const char* location;
    int number_of_games;
    int number_of_players;
    int longest_game_time;
    int total_play_time;

};

MapDataElement copyDataTournament(MapDataElement element)
{
    if (element == NULL)
    {
        return NULL;
    }
    Tournament new_tournament = tournamentCopy((Tournament)element);
    if (new_tournament == NULL)
    {
        return NULL;
    }
    return new_tournament;
}

MapKeyElement copyKeyTournamentID(MapKeyElement element)
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

void freeDataTournament(MapDataElement element)
{
    tournamentDestroy((Tournament)element);
}

void freeKeyTournamentID(MapKeyElement element)
{
    free(element);
}

int compareKeyTournamentID(MapKeyElement element1, MapKeyElement element2)
{
    return *(int*) element1 - *(int*) element2;
}

Map tournamentMapFactory()
{
    Map tournament_map = mapCreate(copyDataTournament, copyKeyTournamentID, freeDataTournament, freeKeyTournamentID, compareKeyTournamentID);
    if (tournament_map == NULL)
    {
        return NULL;
    }
    return tournament_map;
}

Tournament tournamentCreate(int max_games_per_player, const char* location)
{
    Tournament new_tournament = malloc(sizeof(*new_tournament));
    if (new_tournament == NULL)
    {
        return NULL;
    }

    new_tournament->games = gameMapFactory();
    if (new_tournament->games == NULL)
    {
        free(new_tournament);
        return NULL;
    }

    new_tournament->players = playerMapFactory();
    if (new_tournament->players == NULL)
    {
        mapDestroy(new_tournament->games);
        free(new_tournament);
        return NULL;
    }

    new_tournament->winner = TOURNAMENT_NOT_ENDED;
    new_tournament->max_games_per_player = max_games_per_player;
    new_tournament->location = location;
    new_tournament->number_of_games = 0;
    new_tournament->number_of_players = 0;
    new_tournament->longest_game_time = 0;
    new_tournament->total_play_time = 0;

    return new_tournament;
}

void tournamentDestroy(Tournament tournament)
{
    if (tournament == NULL)
    {
        return;
    }

    mapDestroy(tournament->games); 
    mapDestroy(tournament->players);
    tournament->location = NULL;
    tournament->longest_game_time = 0;
    tournament->max_games_per_player = 0;
    tournament->number_of_games = 0;
    tournament->total_play_time = 0;
    tournament->winner = 0;
    free(tournament);
}

Tournament tournamentCopy(Tournament tournament)
{
    if (tournament == NULL)
    {
        return NULL;
    }

    Tournament tournament_cpy = malloc(sizeof(*tournament_cpy));
    if (tournament_cpy == NULL)
    {
        return NULL;
    }

    tournament_cpy->games = mapCopy(tournament->games);
    if (tournament_cpy->games == NULL)
    {
        tournamentDestroy(tournament_cpy);
        return NULL;
    }

    tournament_cpy->players = mapCopy(tournament->players);
    if (tournament_cpy->players == NULL)
    {
        tournamentDestroy(tournament_cpy);
        return NULL;
    }

    tournament_cpy->winner = tournament->winner;
    tournament_cpy->max_games_per_player = tournament->max_games_per_player;
    tournament_cpy->location = tournament->location;
    tournament_cpy->number_of_games = tournament->number_of_games;
    tournament_cpy->number_of_players = tournament->number_of_players;
    tournament_cpy->longest_game_time = tournament->longest_game_time;
    tournament_cpy->total_play_time = tournament->total_play_time;
    
    return tournament_cpy;
}

int tournamentGetWinner(Tournament tournament)
{
    if(tournament == NULL)
    {
        return TOURNAMENT_NOT_EXIST;
    }
    return tournament->winner;
}

void tournamentSetWinner(Tournament tournament, int winner_id)
{
    if(tournament == NULL)
    {
        return;
    }
    tournament->winner = winner_id;
}

Map tournamentGetGamesMap(Tournament tournament)
{
    if (tournament == NULL)
    {
        return NULL;
    }
    return tournament->games;
}

Map tournamentGetPlayersMap(Tournament tournament)
{
    if (tournament == NULL)
    {
        return NULL;
    }
    return tournament->players;
}

int tournamentGetMaxGamesPerPlayer(Tournament tournament)
{
    if (tournament == NULL)
    {
        return TOURNAMENT_NOT_EXIST;
    }
    return tournament->max_games_per_player;
}

void tournamentSetMaxGamesPerPlayer(Tournament tournament, int max)
{
    if (tournament == NULL)
    {
        return;
    }
    tournament->max_games_per_player = max;
}

const char* tournamentGetLocation(Tournament tournament)
{
    if (tournament == NULL)
    {
        return INVALID_LOCATION;
    }
    return tournament->location;
}

void tournamentSetLocation(Tournament tournament, const char *location)
{
    if (tournament == NULL)
    {
        return;
    }
    tournament->location = location;
}

int countGamesPlayerPlayedInTournament(Tournament tournament, int player_id)
{
    if (tournament == NULL)
    {
        return TOURNAMENT_NOT_EXIST; 
    }
    int count = 0;

    MAP_FOREACH (Game, current_game_id, tournament->games)
    {
        Game current_game = mapGet(tournament->games, current_game_id);
        if (isPlayerInCurrentGame(current_game, player_id))
        {
            count++;
        }
        free(current_game_id);
    }
    return count;
}

bool isMaxGamesPerPlayerExceeded(Tournament tournament, int player_id)
{
    return (tournament->max_games_per_player <= countGamesPlayerPlayedInTournament(tournament, player_id));
}

int tournamentGetNumberOfGames(Tournament tournament)
{
    if(tournament == NULL)
    {
        return TOURNAMENT_NOT_EXIST;
    }
    return tournament->number_of_games;
}

void tournamentUpdateNumberOfGames(Tournament tournament)
{
    if(tournament == NULL)
    {
        return;
    }
    tournament->number_of_games++;
}

void tournamentUpdateNumberOfPlayers(Tournament tournament)
{
    if(tournament == NULL)
    {
        return;
    }
    tournament->number_of_players++;
}

int tournamentGetNumberOfPlayers(Tournament tournament)
{
    if(tournament == NULL)
    {
        return TOURNAMENT_NOT_EXIST;
    }
    return tournament->number_of_players;
}

int tournamentGetLongestGameTime(Tournament tournament)
{
    if(tournament == NULL)
    {
        return TOURNAMENT_NOT_EXIST;
    }
    return tournament->longest_game_time;
}

void tournamentUpdateLongestGameTime(Tournament tournament, int game_play_time)
{
    if(tournament == NULL)
    {
        return;
    }
    if (tournament->longest_game_time < game_play_time)
    {
        tournament->longest_game_time = game_play_time;
    }
}

int tournamentGetTotalPlayTime(Tournament tournament)
{
    if(tournament == NULL)
    {
        return TOURNAMENT_NOT_EXIST;
    }
    return tournament->total_play_time;
}

void tournamentUpdateTotalPlayTime(Tournament tournament, int game_play_time)
{
    if(tournament == NULL)
    {
        return;
    }
    tournament->total_play_time = tournament->total_play_time + game_play_time;
}

void tournamentUpdateStats(Tournament tournament, int play_time)
{
    tournamentUpdateNumberOfGames(tournament);
    tournamentUpdateLongestGameTime(tournament, play_time);
    tournamentUpdateTotalPlayTime(tournament, play_time);
}