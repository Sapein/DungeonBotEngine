#include "database.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#if (DE_DATABASE_TYPE == SQLITE3)
#include <sqlite3.h>
#endif

/* Important Functions */
static enum SQL_MODE {NONE, SQLITE3};
static enum SQL_MODE mode = DE_DATABASE_TYPE;
static union {
    sqlite3 *sqlite3db = NULL;
    int nodb;
} database;

enum DungeonEngine_DBErr DungeonEngine_DBInit(void){
    if(SQL_MODE == SQLITE3){
        sqlite3_initalize();
        if(sqlite3_open(DE_DATABASE_PATH, &(database.sqlite3db)) != 0){
            sqlite3_shutdown();
            printf("sqlite3 DB open error: %s\n", sqlite3_errmsg(database.sqlite3db));
            return SQLITE_ERR;
        }
    }else if(SQL_MODE == NONE){
        #if (DE_DATABASE_TYPE == NONE)
        #error *** Not Implemented, use SQLITE3 ***
        #endif
    }
    return SUCCESS;
}

enum DungeonEngine_DBErr DungeonEngine_DBShutdown(void){
    int close_status = SQLITE_OK;
    if(SQL_MODE == SQLITE3){
        do{
            close_status = sqlite3_close(DE_DATABASE_PATH &(database.sqlite3db));
        }while(close_status != SQLITE_OK)
        sqlite3_shutdown();
    }else if(SQL_MODE == NONE){
        #if (DE_DATABASE_TYPE == NONE)
        #error *** Not Implemented, use SQLITE3 ***
        #endif
    }
    return SUCCESS;
}

/* Player Functions */
enum DungeonEngine_DBErr DungeonEngine_DBRegisterPlayer(char playername, long int name_len,
                                                        char password, long int password_len){
    #if (DE_DATABSE_TYPE == SQLITE3)
    #elif (DE_DATABSE_TYPE == NONE)
    #error ** NOT IMPLEMENTED USE SQLITE3 **
    #endif
}

enum DungeonEngine_DBErr DungeonEngine_DBDeletePlayer(char playername, long int name_len,
                                                      char password, long int password_len);
enum DungeonEngine_DBErr DungeonEngine_DBGetPlayerName(char playername, long int name_len, void *player_data);
enum DungeonEngine_DBErr DungeonEngine_DBGetPlayerId(long int player_id, void *player_data);
enum DungeonEngine_DBErr DungeonEngine_DBUpdatePlayer(char playername, long int name_len,
                                                      char password, long int password,
                                                      void *new_player_data);

/* Character Functions */
enum DungeonEngine_DBErr DungeonEngine_DBAddCharacter(char playername, long int name_len,
                                                      char password, long int password_len,
                                                      void *player_data);
enum DungeonEngine_DBErr DungeonEngine_DBDeleteCharacter(char playername, long int name_len,
                                                         char password, long int password_len,
                                                         void *player_data);
enum DungeonEngine_DBErr DungeonEngine_DBGetCharacterName(char playername, long int name_len,
                                                          void *player_data);
enum DungeonEngine_DBErr DungeonEngine_DBGetCharacterId(long int character_id);
enum DungeonEngine_DBErr DungeonEngine_DBUpdatePlayer(char charactername, long int name_len,
                                                      char password, long int password,
                                                      void *new_char_data);


/* Misc. Functions */
enum DungeonEngine_DBErr DungeonEngine_DBQuerrySQL(char msg[], long int msg_len){
    if(SQL_MODE != NONE){
        return SUCCESS;
    }else{
        return NOSQL;
    }
}
