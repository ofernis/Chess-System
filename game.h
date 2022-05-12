#ifndef GAME_H_
#define GAME_H_

#include "map.h"
#include "chessSystem.h"

#define GAME_NOT_EXIST 0

/** Type for representing a game */
typedef struct game_t *Game;

/**
 * copyDataGame: copies a data, whose type is a game, from a given game.
 *
 * @param element - The source game to copy.
 * 
 * @return
 *   A new copy of the given game if success.
 *   NULL - In case of null argument or memory error.
 */
MapDataElement copyDataGame(MapDataElement element);

/**
 * copyKeyGameID: copies a key, whose type is a game ID, from a given game ID.
 *
 * @param element - The source game ID to copy.
 * 
 * @return
 *     A new copy of the given game.
 *     NULL - In case of null argument or memory error.
 */
MapKeyElement copyKeyGameID(MapKeyElement element);

/**
 * freeDataGame: Frees a given Data, which is a game.
 *
 * @param element - The game which needed to be freed.
 * 
 * @return
 *     None
 */
void freeDataGame(MapDataElement element);

/**
 * freeKeyGameID: Frees a given key, which is a game ID.
 *
 * @param element - The key which needed to be freed.
 * 
 * @return 
 *     None
 */
void freeKeyGameID(MapKeyElement element);

/**
 * compareKeyGameID: Compares two game IDs and returns the difference,
 *                   if the first ID is higer - the returned value is positive,
 *                   if the IDs are equal - zero is returned, otherwise negative.
 *
 * @param element1 - The first game ID to compare.
 * @param element2 - The second game ID to compare.
 * 
 * @return 
 *     None
 */
int compareKeyGameID(MapKeyElement element1, MapKeyElement element2);

/**
 * gameMapFactory: Creates a new game map, using the mapCreate function.
 *
 * @param 
 *     None
 * 
 * @return
 *      A new game map if success, otherwise NULL.
 *      NULL - In case of memory error.
 */
Map gameMapFactory();

/**
 * gameCreate: create a game.
 *
 * @param winner - The game's winner.
 * @param play_time - The game's playtime.
 * @param first_player - The game's first player.
 * @param second_player - The game's second player.
 * 
 * @return 
 *     A new game if success, and NULL otherwise (e.g.
 *     in case of an allocation error).
 */
Game gameCreate(Winner winner, int play_time, int first_player, int second_player);

/**
 * gameCopy: copies a given game.
 *
 * @param game - The source game which would be copied.
 * 
 * @return 
 *     A new copy of the given game if success, and NULL otherwise (e.g.
 *     in case of an allocation error)
 */
Game gameCopy(Game game);

/**
 * gameDestroy: Deletes a given game.
 *
 * @param game - The source game which would be deleted.
 * 
 * @return
 *  None
 */
void gameDestroy(Game game);

/**
 * gameGetWinner: Finds the winner of a given game.
 *
 * @param game - The game which to find the winner of.
 * 
 * @return
 *    None
 */
int gameGetWinner(Game game);

/**
 * isPlayerInCurrentGame: Checks if a given player exists in a game.
 *
 * @param game - The game which is being checked.
 * @param player_id - The player who is being checked.
 * 
 * @return
 *     false - If the player is in the game.
 *     true - If the player is not in the game. 
 */
bool isPlayerInCurrentGame(Game game, int player_id);

/**
 * gameGetOpponent: Gets the opponent in a given game.
 *
 * @param game - The game which is being checked.
 * @param opponent_id - The opponent's ID.
 * 
 * @return
 *     The ID of the opponent in the game.
 *     GAME_NOT_EXIST - In case of a NULL argument.
 */
int gameGetOpponent(Game game, int opponent_id);

/**
 * gameSetWinner: Sets the winner a given game.
 *
 * @param game - The game to set a winner to.
 * @param winner_id - The ID of the winner to set.
 * 
 * @return
 *     None
 */
void gameSetWinner(Game game, int winner_id);

/**
 * gameGetPlayTime: Gets the playtime a given game.
 *
 * @param game - The game which is being checked.
 * 
 * @return
 *     The playtime of the game.
 *     GAME_NOT_EXIST - In case of a NULL argument.
 */
int gameGetPlayTime(Game game);

/**
 * gameGetFirstPlayer: Gets the first player in a given game.
 *
 * @param game - The game which is being checked.
 * 
 * @return
 *     The first player in the given game.
 *     GAME_NOT_EXIST - In case of a NULL argument.
 */
int gameGetFirstPlayer(Game game);

/**
 * gameGetSecondPlayer: Gets the second player in a given game.
 *
 * @param game - The game which is being checked.
 * 
 * @return
 *     The second player in the given game.
 *     GAME_NOT_EXIST - In case of a NULL argument.
 */
int gameGetSecondPlayer(Game game);

/**
 * gameUpdateDeletedPlayerID: Updates the ID of a deleted player,
 *                            by changing the sign of the ID to minus. 
 *
 * @param game - The game which is being checked.
 * @param player_id - The ID of the deleted player.
 * 
 * @return
 *     None
 */
void gameUpdateDeletedPlayerID(Game game, int player_id);

#endif /* GAME_H_ */