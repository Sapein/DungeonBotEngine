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
#define DE_GENERATION_HEIGHT 100 /*Y Values*/
#define DE_GENERATION_WIDTH  100 /*X Values*/
#define DE_EXIT_DISTANCE 10 /* Amount of room distance between the Entrance and Exit */

/* Can either be NONE or SQLITE3 */
#define DE_DATABASE_TYPE SQLITE3
#define DE_DATABASE_PATH "dungeonbot.db"

/* File Settings */
#define DE_FILE_MEMORY_SIZE 100 /* Size of the Memory Buffer in amount of Files */

/* Registration Settings */
#define DE_INTERNAL_HASHING OFF /* Can either be OFF or ON */

/************************************
 ***       ADVANCED SETTINGS      ***
 ************************************
 *** !WARNING! DO NOT EDIT UNLESS ***
 ***  YOU KNOW WHAT YOU ARE DOING ***
 ************************************/
#define DE_COORDINATE_BASED off /*on = the dungeon is 'coordinate-based' ; off = the dungeon is not coordindate-based*/
#define DE_DISTANCE_TYPE taxicab /* Distance Formula to use for the distance between Entrance and Exit
                                  * Options: taxicab = use taxicab formula. euclid = use regular distance formula. */
#endif
