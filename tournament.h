#ifndef TOURNAMENT_H_
#define TOURNAMENT_H_

#include "map.h"
#include "game.h"

#define INVALID_LOCATION NULL
#define TOURNAMENT_NOT_ENDED 0
#define TOURNAMENT_NOT_EXIST 0

/* type for representing a tournament */
typedef struct tournament_t *Tournament;


/**
 * copyDataTournament: copies a data, whose type is a tournament, from a given tournament.
 *
 * @param element - The source tournament to copy.
 *
 * @return
 *     a new copy of the given tournament if success.
 *     NULL otherwise (allocation error for example).
 */
MapDataElement copyDataTournament(MapDataElement element);

/**
 * copyKeyTournamentID: copies a key, whose type is tournament id, from a given tournament id
 *
 * @param element - the source tournament id to copy.
 *
 * @return
 *      a new copy of the given tournament id if success.
 *      NULL otherwise.
 */
MapKeyElement copyKeyTournamentID(MapKeyElement element);

/**
 * freeDataTournament: frees a given data, which is a tournament.
 *
 * @param element - the tournament that needs to be freed.
 *
 * @return
 *      none.
 */
void freeDataTournament(MapDataElement element);

/**
 * freeKeyTournamentID: frees a given key, which is a tournament id.
 *
 * @param element - the tournament id that needs to be freed.
 *
 * @return
 *      none.
 */
void freeKeyTournamentID(MapKeyElement element);

/**
 * compareKeyTournamentID: compares two key tournament id's, and returns the difference.
 *                         if the first tournament id is bigger, a positive number is returned.
 *                         if the second tournament id is bigger, a negative number is returned.
 *                         if the tournament id's are equal, zero is returned.
 *
 * @param element1 - the first tournament id to be compared.
 * @param element2 - the second tournament id to be compared.
 *
 * @return
 *      a positive number if the first id is bigger than the second.
 *      a negative number if the second id is bigger that the first.
 *      zero if they are equal.
 */
int compareKeyTournamentID(MapKeyElement element1, MapKeyElement element2);

/**
 * tournamentMapFactory: creates a new tournament map, using the mapCreate function.
 *
 * @param none
 *
 * @return
 *      a new tournament map in case of success.
 *      NULL otherwise.
 */
Map tournamentMapFactory();

/**
 * tournamentCreate: creates a tournament.
 *
 * @param max_games_per_player = the tournament's max games per player.
 * @param location - the tournament's location.
 *
 * @return
 *      a new tournament in case of success.
 *      NULL otherwise.
 */
Tournament tournamentCreate(int max_games_per_player, const char* location);

/**
 * tournamentDestroy: deletes a given tournament.
 *
 * @param tournament - the tournament which needs to be deleted.
 *
 * @return
 *      none.
 */
void tournamentDestroy(Tournament tournament);

/**
 * tournamentCopy: copies a given tournament.
 *
 * @param tournament - the source tournament which needs to be copied
 *
 * @return
 *      a copy of the given tournament in case of success.
 *      NULL otherwise (e.g. allocation error, or if parameter is NULL)
 */
Tournament tournamentCopy(Tournament tournament);

/**
 * tournamentGetGamesMap: finds the map of games of given tournament.
 *
 * @param tournament - the tournament which we need to find the games map for.
 *
 * @return
 *      NULL if tournament is null.
 *      map of games of the given tournament if successful.
 */
Map tournamentGetGamesMap(Tournament tournament);

/**
 * tournamentGetPlayersMap: finds the map of players of given tournament.
 *
 * @param tournament - the tournament which we need to find the players map for.
 *
 * @return
 *      NULL if tournament is null.
 *      map of players of the given tournament if successful.
 */
Map tournamentGetPlayersMap(Tournament tournament);

/**
 * tournamentGetWinner: finds the winner of the given tournament.
 *
 * @param tournament - the tournament which we need to find the winner of.
 *
 * @return
 *      TOURNAMENT_NOT_EXIST if tournament is null
 *      the id of the winner of the given tournament if successful
 */
int tournamentGetWinner(Tournament tournament);

/**
 * tournamentSetWinner: sets the winner for the given tournament.
 *
 * @param tournament - the tournament which we need to set the winner of.
 * @param winner_id - the winner of the given tournament
 *
 * @return
 *      none.
 */
void tournamentSetWinner(Tournament tournament, int winner_id);

/**
 * tournamentGetMaxGamesPerPlayer: finds the max games per player for the given tournament.
 *
 * @param tournament - the tournament which we need to find max games per player for.
 *
 * @return
 *      TOURNAMENT_NOT_EXISTS if tournament parameter is null
 *      the max games per player if successful.
 */
int tournamentGetMaxGamesPerPlayer(Tournament tournament);

/**
 * tournamentSetMaxGamesPerPlayer: sets the max games per player for the given tournament
 *
 * @param tournament - the tournament which we need to set the max games per player for.
 *
 * @return
 *      none.
 */
void tournamentSetMaxGamesPerPlayer(Tournament tournament, int max);

/**
 * tournamentGetLocation: finds the location of the given tournament.
 *
 * @param tournament - the tournament which we need to find the location of.
 *
 * @return
 *      INVALID_LOCATION if tournament is null.
 *      the tournament location if successful.
 */
const char* tournamentGetLocation(Tournament tournament);

/**
 * tournamentSetLocation: sets the location of the given tournament.
 *
 * @param tournament - the tournament which we need to set the location for.
 * @param location - the location of the given tournament.
 *
 * @return
 *      none.
 */
void tournamentSetLocation(Tournament tournament, const char *location);

/**
 *  countGamesPlayerPlayerInTournament: counts the amount of games a player has played in the given tournament.
 *
 * @param tournament - the tournament in which we need to count the games a player has played.
 * @param player_id - the player whose games we are counting.
 *
 * @return
 *      TOURNAMENT_NOT_EXIST if the tournament parameter is null.
 *      the amount of games the player has played in the tournament if successful.
 */
int countGamesPlayerPlayedInTournament(Tournament tournament, int player_id);

/**
 * isMaxGamesPerPlayerExceeded: determines whether the player has played more games than allowed.
 *
 * @param tournament - the tournament in which we are checking if the player played more games than allowed.
 * @param player_id - the player we need to check if played more games than allowed.
 *
 * @return
 *      true - if the player played more games than allowed.
 *      false - if the played played less games or equal games to the max number allowed.
 */
bool isMaxGamesPerPlayerExceeded(Tournament tournament, int player_id);

/**
 * tournamentGetNumberOfGames: gets the number of games that occurred in the given tournament.
 *
 * @param tournament - the tournament which we need to check the anumber of games for.
 *
 * @return
 *      TOURNAMENT_NOT_EXIST if the tournament parameter is null.
 *      the number of games in the tournament if successful.
 */
int tournamentGetNumberOfGames(Tournament tournament);

/**
 * tournamentUpdateNumberOfGames: adds 1 to the amount of games that occurred in the tournament.
 *                                this function is used when adding a new game.
 *
 * @param tournament - the tournament which we need to update the number of games of.
 *
 * @return
 *      none
 */
void  tournamentUpdateNumberOfGames(Tournament tournament);

/**
 * tournamentUpdateNumberOfPlayers: updates the number of players participating in the tournament.
 *                                  this function is used when adding a new player to the tournament.
 *
 * @param tournament - the tournament which we need to update the number of games of.
 *
 * @return
 *      none
 */
void tournamentUpdateNumberOfPlayers(Tournament tournament);

/**
 * tournamentGetNumberOfPlayers: gets the number of players who participated in the tournament
 *
 * @param tournament - the tournament which we need to get the number of players of.
 *
 * @return
 *      TOURNAMENT_NOT_EXIST if the tournament parameter is null.
 *      the number of players that played in the tournament if successful.
 */
int tournamentGetNumberOfPlayers(Tournament tournament);

/**
 * tournamentGetLongestGameTime: gets the time in seconds of the longest game that occurred in the tournament.
 *
 * @param tournament - the tournament which we need to find the longest game of.
 *
 * @return
 *      TOURNAMENT_NOT_EXIST if the tournament parameter is null.
 *      the longest game time in seconds if successful.
 */
int tournamentGetLongestGameTime(Tournament tournament);

/**
 * tournamentUpdateLongestGame: compares a new game's time with the tournaments current longest game time,
 *                              if the new game time is longer, the tournament longest game time will be updated.
 *
 * @param tournament - the tournament whose longest game time needs to be updated
 * @param game_play_time - the new game's time which needs to be compared to the tournament's current longest game time
 *
 * @return
 *      none
 */
void tournamentUpdateLongestGameTime(Tournament tournament, int game_play_time);

/**
 * tournamentGetTotalPlayTime: finds the tournament's total play time.
 *
 * @param tournament - the tournament which we need to find the total play time for.
 *
 * @return
 *      TOURNAMENT_NOT_EXIST if the tournament parameter is null.
 *      the tournament's total play time if successful.
 */
int tournamentGetTotalPlayTime(Tournament tournament);

/**
 * tournamentUpdatePlayTime: adds a new game's time to the total play time of the given tournament.
 *
 * @param tournament - the tournament which we need to update total play time of.
 * @param game_play_time - the new game's play time, which needs to be added to the total play time.
 */
void tournamentUpdateTotalPlayTime(Tournament tournament, int game_play_time);

/**
 * tournamentUpdateStats: updates tournament's number of games, longest game time, and total play time.
 *
 * @param tournament - the tournament which stats' need to be updated
 * @param play_time - the play time which needs to be added to total play time, and compare to longest game time.
 *
 * @return
 *      none
 */
void tournamentUpdateStats(Tournament tournament, int play_time);

#endif /* TOURNAMENT_H_ */