#include "chessSystem.h"
#include "tournament.h"
#include "game.h"
#include "map.h"
#include "player.h"
#include <stdlib.h>
#include <stdio.h>

#define NUMBER_OF_PLAYERS_IN_GAME 2

/** Type for representing a chess system that organizes chess tournaments */
struct chess_system_t
{
    Map tournaments;
    Map players;
};

ChessSystem chessCreate()
{
    ChessSystem chess = malloc(sizeof(*chess));
    if (chess == NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }

    chess->tournaments = tournamentMapFactory();
    if (chess->tournaments == NULL)
    {
        free(chess);
        return CHESS_OUT_OF_MEMORY;
    }

    chess->players = playerMapFactory();
    if (chess->players == NULL)
    {
        mapDestroy(chess->tournaments);
        free(chess);
        return CHESS_OUT_OF_MEMORY;
    }
    return chess;
}

void chessDestroy(ChessSystem chess)
{
    if (chess == NULL)
    {
        return;
    }

    mapDestroy(chess->tournaments);
    mapDestroy(chess->players);
    free(chess);
}

/**
*	isValidID: check validity of a given ID.
*
* @param id - The ID to check validity.
*
* @return
* 	true - If ID is valid.
*   false - If ID is not valid.
*/
static bool isValidID(int id)
{
    return (id > 0);
}

/**
*	isMaxGamesPerPlayerValid: check validity of max games per player for a tournament.
*
* @param max - Max games value to which check validity.
*
* @return
* 	true - If value is valid.
*   false - If value is not valid.
*/
static bool isMaxGamesPerPlayerValid(int max)
{
    return (max > 0);
}

/**
*	isPlayTimeValid: check validity of given play time.
*
* @param time - The play time to which check validity.
*
* @return
* 	true - If time is valid.
*   false - If time is not valid.
*/
static bool isPlayTimeValid(int time)
{
    return (time >= 0);
}

/**
*	isLocationValid: check validity of a given location.
*
* @param str - The location string to which check validity.
*
* @return
* 	true - If location is valid.
*   false - If location is not valid.
*/
static bool isLocationValid(const char *str)
{
    if (*str < 'A' || *str > 'Z')
    {
        return false;
    }

    while (*(++str) != '\0')
    {
        if (*str != ' ' && (*str < 'a' || *str > 'z'))
        {
            return false;
        }
    }
    return true;
}

/**
*	chessOutOfMemoryDestroy: destroying a given chess system and returning a memory error.
*
* @param chess - The chess system which to destroy.
*
* @return
* 	CHESS_OUT_OF_MEMORY - Memory error.
*/
static ChessResult chessOutOfMemoryDestroy(ChessSystem chess)
{
    chessDestroy(chess);
    return CHESS_OUT_OF_MEMORY;
}

/**
*	chessAddTournamentErrorCheck: checks errors for chessAddTournament function and returns
*                                 relevant error value.
*
* @param chess - See chessSystem.h
*
* @return
*   See chessSystem.h
*/
static ChessResult chessAddTournamentErrorCheck(ChessSystem chess, int tournament_id, 
                                      int max_games_per_player, const char *tournament_location)
{
    if (chess == NULL || tournament_location == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (isValidID(tournament_id) == false)
    {
        return CHESS_INVALID_ID;
    }

    if (mapContains(chess->tournaments, &tournament_id) == true)
    {
        return CHESS_TOURNAMENT_ALREADY_EXISTS;
    }

    if (isLocationValid(tournament_location) == false)
    {
        return CHESS_INVALID_LOCATION;
    }

    if (isMaxGamesPerPlayerValid(max_games_per_player) == false)
    {
        return CHESS_INVALID_MAX_GAMES;
    } 
    return CHESS_SUCCESS;
}

ChessResult chessAddTournament(ChessSystem chess, int tournament_id,
                               int max_games_per_player, const char *tournament_location)
{
    ChessResult error_type = chessAddTournamentErrorCheck(chess, tournament_id,
                                                          max_games_per_player, tournament_location);

    if (error_type != CHESS_SUCCESS)
    {
        return error_type;
    } 

    Tournament new_tournament = tournamentCreate(max_games_per_player, tournament_location);
    if (new_tournament == NULL)
    {
        return chessOutOfMemoryDestroy(chess);
    }

    if (mapPut(chess->tournaments, &tournament_id, new_tournament) != MAP_SUCCESS)
    {
        tournamentDestroy(new_tournament);
        return chessOutOfMemoryDestroy(chess);
    }
    tournamentDestroy(new_tournament);
    return CHESS_SUCCESS;
}

/**
*	playersAddStatsToMap: add given players score and playtime to players map.
*
* @param players_map - The players map to which add stats.  
* @param player1 - First player's ID. Must be positive.
* @param player2 - Second player's ID. Must be positive.
* @param winner - indicates the winner in the match.
*
* @return
* 	None
*/
static void playersAddStatsToMap(Map players_map, int player1, int player2, Winner winner, int play_time)
{
    Player first_player = mapGet(players_map, &player1);
    Player second_player = mapGet(players_map, &player2);
    Player players[NUMBER_OF_PLAYERS_IN_GAME] = {first_player, second_player};
    playersAddScore(players, winner);
    playersAddPlayTime(players, play_time);
}

/**
*	isGameExistInTournament: check if the given game exists in the given tournament
*
* @param tournament - The tournament to check. 
* @param player1 - First player's ID. Must be positive.
* @param player2 - Second player's ID. Must be positive.
*
* @return
* 	true - If the given game exists in the tournament.
*   false - If the given game doesn't exist in the tournament.
*/
static bool isGameExistInTournament(Tournament tournament, int player1, int player2)
{
    MAP_FOREACH(Game, current_game_id, tournamentGetGamesMap(tournament))
    {
        Game current_game = mapGet(tournamentGetGamesMap(tournament) ,current_game_id);
        free(current_game_id);
        int first_player = gameGetFirstPlayer(current_game);
        int second_player = gameGetSecondPlayer(current_game);

        if ((first_player == player1 && second_player == player2) ||
            (first_player == player2 && second_player == player1))
        {
            return true;
        }
    }
    return false;
}

/**
*	playerSetupInMap: check if a given player exists in players map - if not, he is added to the map,
*                     else nothing is done. If the players map belongs to a tournament, the function
*                     updates the number of players in the tournament.
*                     
*
* @param tournament - The tournament to update number of players if the players map belongs to a tournament. 
* @param players_map - The players map which to check and update player.  
* @param player_id - player's ID. Must be positive.
*
* @return
* 	MAP_OUT_OF_MEMORY - If there was a memory allocation error.
*   MAP_SUCCESS - If the player setup was done successfully.
*/
static MapResult playerSetupInMap(Tournament tournament, Map player_map, int player_id)
{
    MapResult error_type = MAP_SUCCESS;

    if (mapContains(player_map, &player_id) == false)
    {
        Player player = playerCreate(player_id);
        if (player == NULL)
        {
            return MAP_OUT_OF_MEMORY;
        }

        if(mapPut(player_map, &player_id, player) != MAP_SUCCESS)
        {
            error_type = MAP_OUT_OF_MEMORY;
        }
        playerDestroy(player);

        if (player_map == tournamentGetPlayersMap(tournament))
        {
            tournamentUpdateNumberOfPlayers(tournament);
        }
    }
    return error_type;
}

/**
*	playerMapUpdatePlayers: Updates the players map with the two given players, if they don't exist in the map.
*                     
*
* @param tournament - The tournament to update number of players if the players map belongs to a tournament. 
* @param players_map - The players map which to check and update players.  
* @param first_player - First player's ID. Must be positive.
* @param second_player - Second player's ID. Must be positive.
*
* @return
* 	MAP_OUT_OF_MEMORY - If there was a memory allocation error.
*   MAP_SUCCESS - If the players setup was done successfully.
*/
MapResult playerMapUpdatePlayers(Tournament tournament, Map players_map, int first_player, int second_player)
{
    if (playerSetupInMap(tournament, players_map, first_player) != MAP_SUCCESS)
    {
        return MAP_OUT_OF_MEMORY;
    }
    
    if (playerSetupInMap(tournament, players_map, second_player) != MAP_SUCCESS)
    {
        return MAP_OUT_OF_MEMORY;
    }
    return MAP_SUCCESS;
}

/**
*	newGameSystemAssign: Assigns a new game to the tournament games map and updates the players map with the
*                        two given players, if they don't exist in the map.              
*
* @param tournament - The tournament in which the new game occurs.
* @param chess_players_map - The chess system's general players map, which needed to be updated.
* @param tournament_games_map - The games map of the tournament, where the new game would be assigned to.
* @param tournament_players_map - The players map of the tournament, which needed to be updated.
* @param new_game- The new game to assign to games map.  
* @param new_game_id - New game's id.
* @param first_player - First player's ID. Must be positive.
* @param second_player - Second player's ID. Must be positive.
*
* @return
* 	MAP_OUT_OF_MEMORY - If there was a memory allocation error.
*   MAP_SUCCESS - If the new game was successfully assigned and players map was updated.
*/
static ChessResult newGameSystemAssign(Tournament tournament, Map chess_players_map, 
                                       Map tournament_games_map, Map tournament_players_map,
                                       Game new_game, int new_game_id, int first_player, int second_player)
{
    if (mapPut(tournament_games_map, &new_game_id, new_game) != MAP_SUCCESS ||
        playerMapUpdatePlayers(tournament, chess_players_map, first_player, second_player) != MAP_SUCCESS ||
        playerMapUpdatePlayers(tournament, tournament_players_map, first_player, second_player) != MAP_SUCCESS)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    return CHESS_SUCCESS;
}

/**
*	chessAddGameErrorCheck: checks errors for chessAddGame function and returns
*                                 relevant error value.
*
* @param chess - See chessSystem.h
*
* @return
*   See chessSystem.h
*/
static ChessResult chessAddGameErrorCheck(ChessSystem chess, int tournament_id, int first_player,
                                          int second_player, Winner winner, int play_time)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (isValidID(tournament_id) == false ||
        isValidID(first_player) == false || 
        isValidID(second_player) == false ||
        first_player == second_player)
    {
        return CHESS_INVALID_ID;
    }

    if (mapContains(chess->tournaments, &tournament_id) == false)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }

    Tournament tournament = mapGet(chess->tournaments, &tournament_id);
    if (tournamentGetWinner(tournament) != TOURNAMENT_NOT_ENDED)
    {
        return CHESS_TOURNAMENT_ENDED;
    }

    if (isGameExistInTournament(tournament, first_player, second_player) == true)
    {
        return CHESS_GAME_ALREADY_EXISTS;
    }

    if (isPlayTimeValid(play_time) == false)
    {
        return CHESS_INVALID_PLAY_TIME;
    }

    if (isMaxGamesPerPlayerExceeded(tournament, first_player) == true || 
        isMaxGamesPerPlayerExceeded(tournament, second_player) == true)
    {
        return CHESS_EXCEEDED_GAMES;
    }
    return CHESS_SUCCESS;
}

ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player,
                         int second_player, Winner winner, int play_time)
{
    ChessResult error_type = chessAddGameErrorCheck(chess, tournament_id, first_player,
                                                    second_player, winner, play_time);

    if (error_type != CHESS_SUCCESS)
    {
        return error_type;
    } 
    
    Tournament current_tournament = mapGet(chess->tournaments, &tournament_id);
    Map tournament_games_map = tournamentGetGamesMap(current_tournament);
    Map tournament_players_map = tournamentGetPlayersMap(current_tournament);
    Game new_game = gameCreate(winner, play_time, first_player, second_player);
    if (new_game == NULL)
    {
        return chessOutOfMemoryDestroy(chess);
    }

    int new_game_id = (mapGetSize(tournament_games_map) + 1);
    
    if (newGameSystemAssign(current_tournament, chess->players, tournament_games_map, tournament_players_map,
                            new_game, new_game_id, first_player, second_player) != CHESS_SUCCESS)
    {
        gameDestroy(new_game);
        chessOutOfMemoryDestroy(chess);
    }

    playersAddStatsToMap(chess->players, first_player, second_player, winner, play_time);
    playersAddStatsToMap(tournament_players_map, first_player, second_player, winner, play_time);
    tournamentUpdateStats(current_tournament, play_time);

    gameDestroy(new_game);
    return CHESS_SUCCESS;
}

/**
*	playersRemoveStats: remove given players score and playtime from players map.
*
* @param players_map - The players map from which to remove player stats.  
* @param player1 - First player's ID. Must be positive.
* @param player2 - Second player's ID. Must be positive.
* @param winner - Indicates the winner in the match.
*
* @return
* 	None
*/
static void playersRemoveStats(Map players_map, int player1, int player2, Winner winner, int play_time)
{
    if (isValidID(player1) == false && isValidID(player2) == false)
    {
        return;
    }
    Player first_player = mapGet(players_map, &player1);
    Player second_player = mapGet(players_map, &player2);
    Player players[NUMBER_OF_PLAYERS_IN_GAME] = {first_player, second_player};
    playersRemoveScore(players, winner);
    playersRemovePlayTime(players, play_time);
}

/**
*	chessRemoveTournamentErrorCheck: checks errors for chessRemoveTournament function and returns
*                                 relevant error value.
*
* @param chess - See chessSystem.h
*
* @return
*   See chessSystem.h
*/
static ChessResult chessRemoveTournamentErrorCheck(ChessSystem chess, int tournament_id) 
{
    if (chess == NULL || chess->tournaments == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (isValidID(tournament_id) == false)
    {
        return CHESS_INVALID_ID;
    }

    if (mapContains(chess->tournaments, &tournament_id) == false)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    return CHESS_SUCCESS;
}

ChessResult chessRemoveTournament(ChessSystem chess, int tournament_id)
{
    ChessResult error_type = chessRemoveTournamentErrorCheck(chess, tournament_id);
    if (error_type != CHESS_SUCCESS)
    {
        return error_type;
    }

    Tournament current_tournament = mapGet(chess->tournaments, &tournament_id);

    MAP_FOREACH(Game, current_game_id, tournamentGetGamesMap(current_tournament))
    {
        Game current_game = mapGet(tournamentGetGamesMap(current_tournament) ,current_game_id);
        playersRemoveStats(chess->players, gameGetFirstPlayer(current_game), gameGetSecondPlayer(current_game),
                            gameGetWinner(current_game), gameGetPlayTime(current_game));
        free(current_game_id);
    }

    mapRemove(chess->tournaments, &tournament_id);
    return CHESS_SUCCESS;
}

/**
*	playersMapTechnicalWinUpdateScore: Assigns a technical win to the opponent of the given player and              
*                                      updates players scores.
*
* @param players_map - The players map, which needed to be updated with technical win score.
* @param game- The game which to update.  
* @param player1 - First player's ID. Must be positive.
* @param player2 - Second player's ID. Must be positive.
* @param opponent - The ID of the opponent in the game, who gets a technical win.
* @param winner - The winner of the game before the technical win occurs.
*
* @return
*   None
*/
static void playersMapTechnicalWinUpdateScore(Map players_map, Game game, int player1, int player2,
                                    int opponent, Winner winner)
{
    if (isValidID(player1) == false || isValidID(player2) == false)
    {
        return;
    }
    Player first_player = mapGet(players_map, &player1);
    Player second_player = mapGet(players_map, &player2);
    Player players[NUMBER_OF_PLAYERS_IN_GAME] = {first_player, second_player};
    Winner new_winner = (opponent == player1) ? FIRST_PLAYER : SECOND_PLAYER;
    playersRemoveScore(players, winner);
    playersAddScore(players, new_winner);
}

/**
*	chessRemovePlayerErrorCheck: checks errors for chessRemovePlayer function and returns
*                                 relevant error value.
*
* @param chess - See chessSystem.h
*
* @return
*   See chessSystem.h
*/
static ChessResult chessRemovePlayerErrorCheck(ChessSystem chess, int player_id)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (isValidID(player_id) == false)
    {
        return CHESS_INVALID_ID;
    }

    if (mapContains(chess->players, &player_id) == false)
    {
        return CHESS_PLAYER_NOT_EXIST;
    }
    return CHESS_SUCCESS;
}

ChessResult chessRemovePlayer(ChessSystem chess, int player_id)
{
    ChessResult error_type = chessRemovePlayerErrorCheck(chess, player_id);

    if (error_type != CHESS_SUCCESS)
    {
        return error_type;
    }

    MAP_FOREACH(Tournament, current_tournament_id, chess->tournaments)
    {
        Tournament current_tournament = mapGet(chess->tournaments ,current_tournament_id);
        if (tournamentGetWinner(current_tournament) == TOURNAMENT_NOT_ENDED)
        {
            MAP_FOREACH(Game, current_game_id, tournamentGetGamesMap(current_tournament))
            {
                Game current_game = mapGet(tournamentGetGamesMap(current_tournament) ,current_game_id);
                Map tournament_players_map = tournamentGetPlayersMap(current_tournament);

                if (isPlayerInCurrentGame(current_game, player_id) == true)            
                {
                    int opponent_id = gameGetOpponent(current_game, player_id);
                    if (isValidID(opponent_id) == true)
                    {
                        playersMapTechnicalWinUpdateScore(chess->players, current_game, gameGetFirstPlayer(current_game),
                                                          gameGetSecondPlayer(current_game), opponent_id, 
                                                          gameGetWinner(current_game));
                        
                        playersMapTechnicalWinUpdateScore(tournament_players_map, current_game, 
                                                        gameGetFirstPlayer(current_game),
                                                        gameGetSecondPlayer(current_game), opponent_id, 
                                                        gameGetWinner(current_game));

                        gameSetWinner(current_game, opponent_id);
                    }
                    gameUpdateDeletedPlayerID(current_game, player_id);
                }
                free(current_game_id);
            }
        }
        mapRemove(tournamentGetPlayersMap(current_tournament), &player_id); 
        free(current_tournament_id);
    }
    mapRemove(chess->players, &player_id);
    
    return CHESS_SUCCESS;
}

/** Type for a generic data geting function, which is used for determing the tournament's winner */
typedef int (*getDataFunc)(Player);

/**
*	comparePlayerData: Generically comapre players data and returning the differnce between the datas.                   
*
* @param players[] - Array that contains the two player whose data is being compared.
* @param func - A generic data get function, in order to get the player's desired data.  
*
* @return
* 	difference - The numeric value of the differnce between the two players' data. 
*/
static int comparePlayerData(Player players[], getDataFunc func)
{
    int first_player_data = func(players[FIRST_PLAYER]);
    int second_player_data = func(players[SECOND_PLAYER]);
    int difference = first_player_data - second_player_data;

    return difference;
}

/**
*	paritySign: checks if the given index is even or odd and returns 1 or -1 accordingly.                   
*
* @param index - Index which to check parity
*
* @return
* 	1 - If the index is even.
*  -1 - If the index is odd.
*/
static int paritySign(int index)
{
    return ((index % 2 == 0) - (index % 2 != 0));
}

/**
*	tournamentFindWinner: Finds and returns the winner of the tournament between two given players.
*                         The function compares the players' scores - If different, returns the player
                          with the higher score. Else, it compares the players' loses - If different, 
                          returns the player with the lower number of loses. Else, it compares the players'
                          wins - If different, returns the player with the higher number of wins. Else, it
                          compares the players' ID and returns the player with the lower ID.
*                       
*
* @param first_player - First player to check.
* @param second_player - Second player to check.  
*
* @return
*   The player who won the tournament between the given two.
*/
static Player tournamentFindWinner(Player first_player, Player second_player)
{
    getDataFunc checks[] = {playerGetScore, playerGetLoses, playerGetWins, playerGetID};
    Player players[NUMBER_OF_PLAYERS_IN_GAME] = {first_player, second_player}; 
    int difference = 0, index = 0;

    for (; index < (sizeof(checks) / sizeof(*checks)); index++)
    {
        difference = comparePlayerData(players, checks[index]);
        if (difference != 0)
        {
            break;
        }
    } 
    return players[difference * paritySign(index) < 0];
}

/**
*	chessEndTournamentErrorCheck: checks errors for chessEndTournament function and returns
*                                 relevant error value.
*
* @param chess - See chessSystem.h
*
* @return
*   See chessSystem.h
*/
static ChessResult chessEndTournamentErrorCheck(ChessSystem chess, int tournament_id)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (isValidID(tournament_id) == false)
    {
        return CHESS_INVALID_ID;
    }

    if (mapContains(chess->tournaments, &tournament_id) == false)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }

    Tournament tournament = mapGet(chess->tournaments, &tournament_id);
    if (tournamentGetWinner(tournament) != TOURNAMENT_NOT_ENDED)
    {
        return CHESS_TOURNAMENT_ENDED;
    }

    if (mapGetSize(tournamentGetGamesMap(tournament)) == 0)
    {
        return CHESS_NO_GAMES;
    }
    return CHESS_SUCCESS;
}

ChessResult chessEndTournament(ChessSystem chess, int tournament_id)
{
    ChessResult error_type = chessEndTournamentErrorCheck(chess, tournament_id);
    if (error_type != CHESS_SUCCESS)
    {
        return error_type;
    }

    Tournament tournament = mapGet(chess->tournaments, &tournament_id);
    Map tournament_players_map = tournamentGetPlayersMap(tournament);
    MapKeyElement current_winner_id = mapGetFirst(tournament_players_map);
    Player current_winner = mapGet(tournament_players_map, current_winner_id);
    
    MAP_FOREACH(Player, current_player_id, tournament_players_map)
    {
        Player current_player = mapGet(tournament_players_map ,current_player_id);
        free(current_player_id);
        if (isValidID(playerGetID(current_player)) == false)
        {
            continue;
        }
        current_winner = tournamentFindWinner(current_winner, current_player);
        
    }
    tournamentSetWinner(tournament, playerGetID(current_winner));
    free(current_winner_id);
    return CHESS_SUCCESS;
}

/**
*	chessCalculateAveragePlayTimeErrorCheck: checks errors for chessCalculateAveragePlayTime
*                                            function and returns relevant error value.
*
* @param chess - See chessSystem.h
*
* @return
*   See chessSystem.h
*/
ChessResult chessCalculateAveragePlayTimeErrorCheck(ChessSystem chess, int player_id)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (isValidID(player_id) == false)
    {
        return CHESS_INVALID_ID;
    }

    if (mapContains(chess->players, &player_id) == false)
    {
        return CHESS_PLAYER_NOT_EXIST;
    }
    return CHESS_SUCCESS;
}

double chessCalculateAveragePlayTime(ChessSystem chess, int player_id, ChessResult *chess_result)
{
    *chess_result = chessCalculateAveragePlayTimeErrorCheck(chess, player_id);
    if (*chess_result != CHESS_SUCCESS)
    {
        return *chess_result;
    }

    Player player = mapGet(chess->players, &player_id);
    double total_play_time = playerGetTotalPlayTime(player);
    if(total_play_time == 0)
    {
        return total_play_time;
    }
    double wins = playerGetWins(player);
    double loses = playerGetLoses(player);
    double draws = playerGetDraws(player);
    return total_play_time / (wins + loses + draws);
}

ChessResult chessSavePlayersLevels(ChessSystem chess, FILE *file)
{
    if(chess == NULL || file == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    Map players_rank_map = playersRankMapFactory();
    if (players_rank_map == NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }

    MAP_FOREACH(Player, current_player_id, chess->players)
    {
        Player current_player = mapGet(chess->players ,current_player_id);
        if (playerGetTotalPlayTime(current_player) == 0)
        {
            free(current_player_id);
            continue;
        }
        double level = playerGetLevel(current_player);
        int id = playerGetID(current_player);

        Rank current_player_rank = playerRankCreate(level , id);
        if (current_player_rank == NULL)
        {
            mapDestroy(players_rank_map);
            return CHESS_OUT_OF_MEMORY;
        }

        if(mapPut(players_rank_map, current_player_rank, current_player) != MAP_SUCCESS)
        {
            mapDestroy(players_rank_map);
            return CHESS_OUT_OF_MEMORY;
        }
        playerRankDestroy(current_player_rank);
        free(current_player_id);
    }

    MAP_FOREACH(Rank, current_player_rank, players_rank_map)
    {
        if (fprintf(file, "%d %.2lf\n", playerRankGetID(current_player_rank),
                                        playerRankGetLevel(current_player_rank)) < 0)
        {
            mapDestroy(players_rank_map);
            return CHESS_SAVE_FAILURE;
        }
        free(current_player_rank);
    }
    mapDestroy(players_rank_map);
    return CHESS_SUCCESS;
}

/**
*	isAnyTournamentEnded: checks if any tournament in the chess system was ended.
*
* @param chess - The chess system which is being checked.
*
* @return
* 	true - If a ended tournament was found.
*   false - If no ended tournament was found.
*/
static bool isAnyTournamentEnded(ChessSystem chess)
{
    MAP_FOREACH(Tournament, current_tournament_id, chess->tournaments)
    {
        Tournament current_tournament = mapGet(chess->tournaments ,current_tournament_id);
        free(current_tournament_id);
        if (tournamentGetWinner(current_tournament) != TOURNAMENT_NOT_ENDED)
        {
            return true;
        } 
    }
    return false;
}


ChessResult chessSaveTournamentStatistics(ChessSystem chess, char *path_file)
{
    if (chess == NULL || path_file == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (isAnyTournamentEnded(chess) == false)
    {
        return CHESS_NO_TOURNAMENTS_ENDED;
    }

    FILE* file_tournament_stats = fopen(path_file, "w");
    if (!file_tournament_stats)
    {
        return CHESS_SAVE_FAILURE;
    }
    MAP_FOREACH(Tournament, current_tournament_id, chess->tournaments)
    {
        Tournament current_tournament = mapGet(chess->tournaments ,current_tournament_id);
        int winner = tournamentGetWinner(current_tournament);
        if (isValidID(winner) == false)
        {
            free(current_tournament_id); 
            continue;
        }
        int longest_game_time = tournamentGetLongestGameTime(current_tournament);
        const char* location = tournamentGetLocation(current_tournament);
        int number_of_games = tournamentGetNumberOfGames(current_tournament);
        double average_game_time = 0;
        if (number_of_games != 0)
        {
            average_game_time = (double) tournamentGetTotalPlayTime(current_tournament) / (double) number_of_games;
        }
        int number_of_players = tournamentGetNumberOfPlayers(current_tournament);
        
        if (fprintf(file_tournament_stats, "%d\n"
                          "%d\n"
                          "%.2lf\n"
                          "%s\n"
                          "%d\n"
                          "%d\n",
                          winner,
                          longest_game_time,
                          average_game_time,
                          location,
                          number_of_games,
                          number_of_players) < 0)
        {
            return CHESS_SAVE_FAILURE;
        }    
        free(current_tournament_id);        
    }
        fclose(file_tournament_stats);
        return CHESS_SUCCESS;
}