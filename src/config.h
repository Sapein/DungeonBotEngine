#ifndef DECONFIG
#define DECONFIG

#define DE_MAX_CHARACTERS_LOADABLE  10
#define DE_MAX_PLAYERS_IN_DUNGEON 5

/* Dungeon Generation Constants */
/* DE_MAX_ROOMS is the absolute maximum number of rooms that can be generated */
/* DE_GENERATION_HEIGHT and DE_GENERATION_WIDTH are used in the generaiton process,
 *  it is advised to make sure that DE_GENERATION_HEIGHT plus DE_GENERATION_WIDTH are not less
 *  than DE_MAX_ROOMS */
#define DE_MAX_ROOMS 100
#define DE_GENERATION_HEIGHT 100
#define DE_GENERATION_WIDTH  100

/* Can either be NONE or SQLITE3 */
#define DE_DATABASE_TYPE SQLITE3
#define DE_DATABASE_PATH "dungeonbot.db"

/* Registration Settings */
#define DE_INTERNAL_HASHING OFF /* Can either be OFF or ON */
#endif
