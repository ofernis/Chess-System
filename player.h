#ifndef PLAYER_H_
#define PLAYER_H_

#define PLAYER_NOT_EXIST 0

/* type for representing a player */
typedef struct player_t *Player;

/* type for representing a player's rank */
typedef struct rank_t *Rank;

/**
 * copyDataPlayer: copies a data, whose type is Player, from a given player.
 *
 * @param element - the Player we want to copy.
 *
 * @return
 *      a new copy of the given player in case of success.
 *      NULL - in case of an allocation error or a null argument.
 */
MapDataElement copyDataPlayer(MapDataElement element);

/**
 * copyKeyPlayerId: copies a key, whose type is player id, from a given player id.
 *
 * @param element - the source player id to be copied.
 *
 * @return
 *      a new copy of the given player id in case of success.
 *      NULL - in case of an allocation error or a null argument.
 */
MapKeyElement copyKeyPlayerID(MapKeyElement element);

/**
 * copyKeyRank: copies a key, whose type is Rank, from a given rank.
 *
 * @param element - the source rank to be copied.
 *
 * @return
 *      a new copy of the given rank in case of success.
 *      NULL - in case of an allocation error or a null argument.
 *
 */
MapKeyElement copyKeyRank(MapKeyElement element);

/**
 * freeDataPlayer: frees a given data, which is a Player.
 *
 * @param element - the player which needs to be freed.
 *
 * @return
 *      none
 */
void freeDataPlayer(MapDataElement element);

/**
 * freeKeyPlayerId: frees a given key, which is a player id.
 *
 * @param element - the key which needs to be freed
 *
 * @return
 *      none
 */
void freeKeyPlayerID(MapKeyElement element);

/**
 * freeKeyRank: frees a given key, which is a rank.
 *
 * @param element - the key which needs to be freed.
 *
 * @return
 *      none.
 */
void freeKeyRank(MapKeyElement element);

/**
 * compareKeyPlayerID: compares two player ID's and returns the difference.
 *                     if the first player id is bigger - the returned value is positive.
 *                     if the second player id is bigger - the returned value is negative.
 *                     if the ID's are equal - the returned value is zero.
 *
 * @param element1 - the first player ID to compare.
 * @param element2 - the second player ID to compare.
 *
 * @return
 *      positive int - if the first player id is bigger.
 *      negative int - if the second player id is bigger.
 *      zero - if both player id's are equal.
 */
int compareKeyPlayerID(MapKeyElement element1, MapKeyElement element2);

/**
 * compareKeyRank: compares two player's rank levels and returns the difference.
 *                 returns a positive number if the first player's rank is bigger than the second.
 *                 returns a negative number if the second player's rank is bigger than the first.
 *                 if both players' rank's are equal, the difference between both player id's is returned,
 *                 meaning a positive number if the first player id is bigger,
 *                 a negative number if the second player id is bigger.
 *                 zero if the player id's are equal
 *
 * @param element1 - the first player rank to compare.
 * @param element2 - the second player rank to compare.
 *
 * @return
 *      positive number - if either the first player rank is bigger than the second player rank,
 *                        or if the first player id is bigger than the second player id.
 *      negative number - if either the second player rank is bigger than the second player rank,
 *                        or if the second player id is bigger than the second player id.
 *      zero - if both player id's are equal.
 */
int compareKeyRank(MapKeyElement element1, MapKeyElement element2);

/**
 * playerMapFactory: Creates a new player map, using the mapCreate function.
 *
 * @param
 *     None
 *
 * @return
 *      A new player map if success, otherwise NULL.
 *      NULL - In case of memory error.
 */
Map playerMapFactory();

/**
 * playersRankMapFactory: Creates a new player's rank map, using the mapCreate function.
 *
 * @param
 *     None
 *
 * @return
 *      A new player rank map if success, otherwise NULL.
 *      NULL - In case of memory error.
 */
Map playersRankMapFactory();

/**
 * playerCreate: creates a new player.
 *
 * @param id - the new players' id.
 *
 * @return
 *     NULL - in case of an allocation error.
 *     a new player if success.
 */
Player playerCreate(int id);

/**
 * PlayerCopy: copies a given player.
 *
 * @param player - the source player which would be copied.
 *
 * @return
 *      NULL - in case of an allocation error.
 *      a new copy of the given player in case of success.
 */
Player playerCopy(Player player);

/**
 * playerDestroy: deletes a given player.
 *
 * @param player - the source player which would be deleted.
 *
 * @return
 *      none
*/
void playerDestroy(Player player);

/**
 * playerRankCreate: creates a new player rank.
 *
 * @param id - the players' id.
 * @param level - the players' level.
 *
 * @return
 *      a new rank in case of success.
 *      NULL otherwise.
*/
Rank playerRankCreate(double level, int id);

/**
 * playerRankDestroy: deletes a given player rank.
 *
 * @param rank - the source player rank which would be deleted
 *
 * @return
 *      none
*/
void playerRankDestroy(Rank rank);

/**
 * playerRankGetID: finds the player id of a given rank.
 *
 * @param rank - player rank which we need to find id of.
 *
 * @return
 *      an int number - which is the player id
*/
int playerRankGetID(Rank rank);

/**
 * playerRankGetLevel: finds the player level of a given rank.
 *
 * @param rank - player rank which we need to find level of.
 *
 * @return
 *      an int number - which is the player level.
*/
double playerRankGetLevel(Rank rank);

/**
 * playerGetID: finds the player id of a given player
 *
 * @param player - the player which we need to find id of.
 *
 * @return
 *      PLAYER_NOT_EXIST - if the player paramater is null.
 *      an int number - which is the player id otherwise
*/
int playerGetID(Player player);

/**
 * playerSetID: sets the id of the given player.
 *              if the player does not exist, nothing will happen.
 *
 * @param player - the player to set the id to.
 * @param id - the id which we need to set to the player.
 *
 * @return
 *      none.
*/
void playerSetID(Player player, int id);

/**
 * playerGetWins: finds the amount of wins the given player has.
 *
 * @param player - the player we need to find amount of wins for.
 *
 * @return
 *      PLAYER_NOT_EXIST - if the player parameter is NULL.
 *      number of wins for the given player otherwise.
*/
int playerGetWins(Player player);

/**
 * playerGetLoses: finds the amount of losses the given player has.
 *
 * @param player - the player we need to find the amount of losses for.
 *
 * @return
 *      PLAYER_NOT_EXIST - if the player parameter is NULL.
 *      number of losses for given player otherwise.
*/
int playerGetLoses(Player player);

/**
 * playerGetDraws: finds the amount of draws the given player has.
 *
 * @param player - the player we need to find the amount of draws for.
 *
 * @return
 *      PLAYER_NOT_EXIST - if the player parameter is NULL.
 *      number of draws for given player otherwise.
*/
int playerGetDraws(Player player);

/**
 * playerGetTotalPlayTime: finds the total play time the given player has.
 *
 * @param player - the player we need to find total play time for.
 *
 * @return
 *      PLAYER_NOT_EXIST - if the player parameter is null.
 *      total play time in seconds for given player otherwise.
*/
int playerGetTotalPlayTime(Player player);

/**
 * playerGetScore: calculates the given players' score based the number of wins and draws the player has.
 *
 * @param player - the player which we need to calculate score for.
 *
 * @return
 *      the score for the given player
*/
int playerGetScore(Player player);

/**
 * playerAddScore: updates players' wins, losses, or draws based on the game winner.
 *
 * @param players[] - an array containing two players who competed against each other in a specific game.
 * @param score - contains the winner of the game.
 *
 * @return
 *      none
*/
void playersAddScore(Player players[], Winner score);

/**
 * playerGetLevel: calculates a given players' level based on the number of wins, draws, and losses,
 *                 and based on the total amount of games played by given player.
 *
 * @param player - the player which we need to calculate level for.
 *
 * @return
 *      Level of given player.
*/
double playerGetLevel(Player player);

/**
 * playersRemoveScore: removes players' wins, losses, or draws from their scores.
 *                     this function is used in removing a tournament from the chess sysytem,
 *                     and therefore removes each players' scores as if the tournament didnt happen.
 *
 * @param players[] - an array of 2 players who competed against each other in a specific game.
 * @param score - contains the winner of the specific game between those 2 players.
 *
 * @return
 *      none
*/
void playersRemoveScore(Player players[], Winner score);

/**
 * playerAddPlayTime: updates players' total play time, after each game.
 *
 * @param players[] - an array containing two players who competed against each other in a specific game.
 * @param play_time - contains the time the game lasted in seconds.
 *
 * @return
 *      none
*/
void playersAddPlayTime(Player players[], int play_time);

/**
 * playerRemovePlayTime: updates players' time from their total time, is used in removing tournaments,
 *                       and therefore removes time of each game that occured in the tournament from
 *                       each player.
 *
 * @param players[] - an array containing two players who competed against each other in a specific game.
 * @param play_time - contains the time the game lasted in seconds.
 *
 * @return
 *      none
*/
void playersRemovePlayTime(Player players[], int play_time);


#endif /* PLAYER_H_ */