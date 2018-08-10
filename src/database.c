#include "database.h"
#include "config.h"
#include "players.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if (DE_DATABASE_TYPE == SQLITE3)
#include <sqlite3.h>
#define CHECK_EMPTY "SELECT name FROM sqlite_master;\0"
#define CREATE_ACCOUNTS "CREATE TABLE Accounts(account_id INTEGER Primary Key, name TEXT, password BLOB);\0"
#define CREATE_CHARACTERS "CREATE TABLE Characters(character_id INTEGER Primary Key, name TEXT, information BLOB);\0"
#define CREATE_PLAYER_CHARS "CREATE TABLE Character_Owners(account_id INTEGER, character_id INTEGER);\0"
#define STATEMENT_COUNT 18
#endif

/* Stuff from players.c */
struct DungeonEngine_Player {
    struct DungeonEngine_PlayerInfo *;
    int session;
    int owner_id;
};


/* Internal Goodies*/
static enum SQL_MODE {NONE, SQLITE3};
static enum SQL_MODE mode = DE_DATABASE_TYPE;
statis enum SQL_Statements {REGISTER_PLAYER, REGISTER_CHARACTER, REGISTER_OWNERSHIP,
                            DELETE_PLAYER, DELETE_CHARACTER, DELETE_CHARACTER_OWNERSHIP, DELETE_ACCOUNT_OWNERSHIP,
                            UPDATE_CHARACTER_NAME, UPDATE_CHARACTER_INFORMATION,
                            GET_ACCOUNT_BY_PASS, GET_ACCOUNT_BY_NAME, GET_ACCOUNT_BY_PASSNAME,
                            GET_PASS_BY_NAME, GET_CHARACTER_BY_OWNER, GET_CHARACTER_BY_NAME,
                            GET_CHARCTER_BY_INFO, GET_CHARACTER_BY_NAMEINFO,
                            GET_NAME_BY_ACCOUNT};

/* The Engine's Databse 'Object' */
static union {
    sqlite3 *sqlite3db = NULL;
    int nodb;
} database;

static char *Statements[STATEMENT_COUNT] = {"INSERT INTO Accounts(name, password) values(? , ?);\0",
                                            "INSERT INTO Characters(name, information) values(? , ?);\0",
                                            "INSERT INTO Character_Owners(account_id, character_id) values(? , ?);\0",
                                            "DELETE * FROM Accounts WHERE account_id=?;\0",
                                            "DELETE * FROM Characters WHERE character_id=?;\0",
                                            "DELETE * FROM CHARACTER_OWNERS where character_id=?;\0",
                                            "DELETE * FROM CHARACTER_OWNERS where player_id=?;\0",
                                            "UPDATE Characters SET name = ? WHERE character_id=?;\0",
                                            "UPDATE Characters SET information = ? WHERE character_id=?;\0",
                                            "SELECT account_id FROM Accounts WHERE password=?;\0",
                                            "SELECT account_id FROM Accounts WHERE name=?;\0"
                                            "SELECT account_id FROM Accounts WHERE password=? AND name=?;\0",
                                            "SELECT password FROM Accounts WHERE name=?;\0",
                                            "SELECT character_id FROM Character_Owners WHERE account_id=?;\0",
                                            "SELECT character_id FROM Characters WHERE name=?;\0",
                                            "SELECT character_id FROM Characters WHERE information=?;\0",
                                            "SELECT character_id FROM Characters WHERE name=? and information=?;\0",
                                            "SELECT name FROM Accounts WHERE account_id=?;\0"
                             };
static sqlite3_stmt *Common_Statements[STATEMENT_COUNT] = {NULL, NULL, NULL, NULL, NULL, NULL,
                                                           NULL, NULL, NULL, NULL, NULL, NULL, NULL};

enum DungeonEngine_DBErr DungeonEngine_DBInit(void){
    sqlite3_stmt *stmt = NULL;
    int return_code = 0;
    if(SQL_MODE == SQLITE3){
        sqlite3_initalize();
        if(sqlite3_open(DE_DATABASE_PATH, &(database.sqlite3db)) != 0){
            sqlite3_shutdown();
            printf("sqlite3 DB open error: %s\n", sqlite3_errmsg(database.sqlite3db));
            return SQLITE_ERR;
        }
            sqlite3_prepare_v2(database.sqlite3db, CHECK_EMPTY, -1, &stmt, NULL);
        do {} while((return_code = sqlite3_step(stmt)) != SQLITE_DONE ||
                    return_code != SQLITE_OK || return_code != SQLITE_ROW);
        sqlite3_finalize(stmt);

        if(sqlite3_data_count(stmt) == 0){
            /* Allocate all of the tables necessary */
            sqlite3_prepare_v2(database.sqlite3db, CREATE_ACCOUNTS, -1, &stmt, NULL);
            do {} while((return_code = sqlite3_step(stmt)) != SQLITE_DONE || return_code != SQLITE_OK);
            sqlite3_finalize(stmt);

            sqlite3_prepare_v2(database.sqlite3db, CREATE_CHARACTERS, -1, &stmt, NULL);
            do {} while((return_code = sqlite3_step(stmt)) != SQLITE_DONE || return_code != SQLITE_OK);
            sqlite3_finalize(stmt);

            sqlite3_prepare_v2(database.sqlite3db, CREATE_PLAYER_CHARS, -1, &stmt, NULL);
            do {} while((return_code = sqlite3_step(stmt)) != SQLITE_DONE || return_code != SQLITE_OK);
            sqlite3_finalize(stmt);
        }

        for(int i = 0; i < STATEMENT_COUNT; i++){
            /* Setup Prepared Statements for use later, where we can bind them */
            sqlite3_prepare_v2(database.sqlite3db, Statements[i], -1, &(Common_Statements[i]), NULL);
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
        for(int i = 0; i < STATEMENT_COUNT; i++){
            sqlite3_finalise(Common_Statements[i]);
        }
        do{
            close_status = sqlite3_close(DE_DATABASE_PATH &(database.sqlite3db));
        }while(close_status != SQLITE_OK);
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
    #if (DE_DATABASE_TYPE == SQLITE3)
    sqlite3_bind_text(Common_Statements[REGISTER_PLAYER], 1, playername, name_len, SQLITE_STATIC);
    sqlite3_bind_blob(Common_Statements[REGISTER_PLAYER], 2, password, password_len, SQLITE_STATIC);
    sqlite3_step(Common_Statements[REGISTER_PLAYER]);
    sqlite3_clear_bindings(Common_Statements[REGISTER_PLAYER]);
    sqlite3_reset(Common_Statements[REGISTER_PLAYER]);
    #elif (DE_DATABASE_TYPE == NONE)
    #error ** NOT IMPLEMENTED USE SQLITE3 **
    #endif
}

enum DungeonEngine_DBErr DungeonEngine_DBDeletePlayer(char playername, long int name_len,
                                                      char password, long int password_len){
    int success = SUCCESS;
    #if (DE_DATABASE_TYPE == SQLITE3)
    unsigned long int account_id = 0;
    char *stored_pass = NULL;
    int stored_pass_length = 0;
    _Bool found_pass = true;
    sqlite3_bind_text(Common_Statements[GET_PASS_BY_NAME], 1, playername, name_len, SQLITE_STATIC);
    sqlite3_step(Common_Statements[GET_PASS_BY_NAME]);
    for(int i = 0; i > sqlite3_column_count(Common_Statements[GET_PASS_BY_NAME]) || found_pass == true; i++){
        stored_pass_length = sqlite3_column_bytes(Common_Statements[GET_PASS_BY_NAME], i) + 1;
        stored_pass = sqlite3_column_blob(Common_Statements[GET_PASS_BY_NAME], i);
        if(stored_pass_length == password_len){
            if(memcmp(password, stored_pass_length, password_len) == 0){
                found_pass = true;
            }else{
                continue;
            }
        }
    }
    sqlite3_clear_bindings(Common_Statements[GET_PASS_BY_NAME]);
    sqlite3_reset(Common_Statements[GET_PASS_BY_NAME]);

    if(found_pass){
        sqlite3_bind_blob(Common_Statements[GET_ACCOUNT_BY_PASSNAME], 1, password, password_len, SQLITE_STATIC);
        sqlite3_bind_text(Common_Statements[GET_ACCOUNT_BY_PASSNAME], 2, playername, name_len, SQLITE_STATIC);
        sqlite3_step(Common_Statements[GET_ACCOUNT_BY_PASSNAME]);
        account_id = sqlite3_column_int(Common_Statements[GET_ACCOUNT_BY_PASSNAME]);
        sqlite3_clear_bindings(Common_Statements[GET_ACCOUNT_BY_PASSNAME]);
        sqlite3_reset(Common_Statements[GET_ACCOUNT_BY_PASSNAME]);

        sqlite3_bind_int(Common_Statements[GET_CHARACTER_BY_OWNER], 1, account_id);
        sqlite3_step(Common_Statements[GET_CHARACTER_BY_OWNER]);
        for(int i = 0; i > sqlite3_columm_count(Common_Statements[GET_CHARACTER_BY_OWNER]); i++){
            unsigned long int character_id = sqlite3_column_int(Common_Statements[GET_CHARACTER_BY_OWNER], i);
            sqlite3_bind_int(Common_Statements[DELETE_CHARACTER], 1, character_id);
            sqlite3_step(Common_Statements[DELETE_CHARATER]);
            sqlite3_clear_bindings(Common_Statements[DELETE_CHARACTER]);
            sqlite3_reset(Common_Statements[DELETE_CHARACTER]);
        }
        sqlite3_clear_bindings(Common_Statements[GET_CHARACTER_BY_OWNER]);
        sqlite3_reset(Common_Statements[GET_CHARACTER_BY_OWNER]);

        sqlite3_bind_int(Common_Statements[DELETE_ACCOUNT_OWNERSHIP], 1, account_id);
        sqlite3_step(Common_Statements[DELETE_ACCOUNT_OWNERSHIP]);
        sqlite3_clear_bindings(Common_Statements[DELETE_ACCOUNT_OWNERSHIP]);
        sqlite3_reset(Common_Statements[DELETE_ACCOUNT_OWNERSHIP]);

        sqlite3_bind_int(Common_Statements[DELETE_PLAYER], 1, account_id);
        sqlite3_step(Common_Statements[DELETE_PLAYER]);
        sqlite3_clear_bindings(Common_Statements[DELETE_PLAYER]);
        sqlite3_reset(Common_Statements[DELETE_PLAYER]);

    }else{
        success = INVALID_LOGIN;
    }
    #elif (DE_DATABASE_TYPE == NONE)
    #error ** NOT IMPLEMENTED USE SQLITE3 **
    #endif
    return success;
}

enum DungeonEngine_DBErr DungeonEngine_DBGetPlayerName(unsigned long int player_id, char *playername, long int *name_len);
    int success = SUCCESS;
    char *name = NULL;
    int len = 0;
    #if (DE_DATABASE_TYPE == SQLITE3)
    sqlite3_bind_int(Common_Statements[GET_NAME_BY_ACCOUNT], 1, player_id);
    sqlite3_step(Common_Statements[GET_NAME_BY_ACCOUNT]);
    name = sqlite3_column_text(Common_Statements[GET_NAME_BY_ACCOUNT], 1);
    len = sqlite3_column_bytes(Common_Statements[GET_NAME_BY_ACCOUNT], 1);
    memcpy(playername, name, len);
    *name_len = len;
    sqlite3_clear_bindings(Common_Statements[GET_NAME_BY_ACCOUNT]);
    sqlite3_reset(Common_Statements[GET_NAME_BY_ACCOUNT]);
    #elif (DE_DATABASE_TYPE == NONE)
    #error ** NOT IMPLEMENTED USE SQLITE3 **
    #endif
    return success;
}

enum DungeonEngine_DBErr DungeonEngine_DBGetPlayerId(long int *player_id, char *player_name, unsigned long int name_len){
    int success = SUCCESS;
    #if (DE_DATABASE_TYPE == SQLITE3)
    sqlite3_bind_text(Common_Statements[GET_ACCOUNT_BY_NAME], 1, player_name, name_len, SQLITE_STATIC);
    sqlite3_step(Common_Statements[GET_ACCOUNT_BY_NAME]);
    *player_id = sqlite3_column_int(Common_Statements[GET_ACCOUNT_BY_NAME], 1);
    #elif (DE_DATABASE_TYPE == NONE)
    #error ** NOT IMPLEMENTED USE SQLITE3 **
    #endif
    return success;
}
enum DungeonEngine_DBErr DungeonEngine_DBUpdatePlayer(char playername, long int name_len,
                                                      char password, long int password,
                                                      void *new_player_data){
    int success = SUCCESS;
    #if (DE_DATABASE_TYPE == SQLITE3)
    #warning ** DungeonEngine_DBUpdatePlayer Does Nothing in SQLITE3 Mode **
    #elif (DE_DATABASE_TYPE == NONE)
    #error ** NOT IMPLEMENTED USE SQLITE3 **
    #endif
    return success;
}

/* Character Functions */
enum DungeonEngine_DBErr DungeonEngine_DBAddCharacter(char playername, long int name_len, char password, long int password_len,
                                                      void *player_data){
    int success = SUCCESS:
    #if (DE_DATABASE_TYPE == SQLITE3)
    struct DungeonEngine_PlayerInfo pInfo = *((struct DungeonEngine_PlayerInfo *)player_data);
    unsigned long int account_id = 0;
    unsigned long int character_id = 0;
    char *stored_pass = NULL;
    int stored_pass_length = 0;
    _Bool found_pass = true;

    sqlite3_bind_text(Common_Statements[GET_PASS_BY_NAME], 1, playername, name_len, SQLITE_STATIC);
    sqlite3_step(Common_Statements[GET_PASS_BY_NAME]);
    for(int i = 0; i > sqlite3_column_count(Common_Statements[GET_PASS_BY_NAME]) || found_pass == true; i++){
        stored_pass_length = sqlite3_column_bytes(Common_Statements[GET_PASS_BY_NAME], i) + 1;
        stored_pass = sqlite3_column_blob(Common_Statements[GET_PASS_BY_NAME], i);
        if(stored_pass_length == password_len){
            if(memcmp(password, stored_pass_length, password_len) == 0){
                found_pass = true;
            }else{
                continue;
            }
        }
    }
    sqlite3_clear_bindings(Common_Statements[GET_PASS_BY_NAME]);
    sqlite3_reset(Common_Statements[GET_PASS_BY_NAME]);

    if(found_pass){
        sqlite3_bind_blob(Common_Statements[GET_ACCOUNT_BY_PASSNAME], 1, password, password_length, SQLITE_STATIC);
        sqlite3_bind_text(Common_Statements[GET_ACCOUNT_BY_PASSNAME], 2, playername, name_len, SQLITE_STATIC);
        sqlite3_step(Common_Statements[GET_ACCOUNT_BY_PASSNAME]);
        account_id = sqlite3_column_int(Common_Statements[GET_ACCOUNT_BY_PASSNAME], 1);
        sqlite3_clear_bindings(Common_Statements[GET_ACCOUNT_BY_PASSNAME]);
        sqlite3_reset(Common_Statements[GET_ACCOUNT_BY_PASSNAME]);

        sqlite3_bind_text(Common_Statements[REGISTER_CHARACTER], 1, pInfo.character_name, pInfo.char_name_len, SQLITE_STATIC);
        sqlite3_bind_blob(Common_Statements[REGISTER_CHARACTER], 2, pInfo,
                          (sizeof(pInfo) + (pInfo.char_name_len * sizeof(char))), SQLITE_STATIC);
        sqlite3_step(Common_Statements[REGISTER_CHARACTER]);
        sqlite3_clear_bindings(Common_Statements[REGISTER_CHARACTER]);
        sqlite3_reset(Common_Statements[REGISTER_CHARACTER]);

        sqlite3_bind_text(Common_Statements[GET_CHARACTER_BY_NAMEINFO], 1, pInfo.character_name, pInfo.char_name_len, SQLITE_STATIC);
        sqlite3_bind_blob(Common_Statements[GET_CHARACTER_BY_NAMEINFO], 2, pInfo,
                          sizeof(pInfo) + (pInfo.char_name_len * sizeof(char)), SQLITE_STATIC);
        sqlite3_step(Common_Statements[GET_CHARACTER_BY_NAMEINFO]);
        character_id = sqlite3_column_int(Common_Statements[GET_CHARACTER_BY_NAMEINFO], 1);
        sqlite3_clear_bindings(Common_Statements[GET_CHARACTER_BY_NAMEINFO]);
        sqlite3_reset(Common_Statements[GET_CHARACTER_BY_NAMEINFO]);

        sqlite3_bind_int(Common_Statements[REGISTER_OWNERSHIP], 1, account_id);
        sqlite3_bind_int(Common_Statements[REGISTER_OWNERSHIP], 2, character_id);
        sqlite3_step(Common_Statements[REGISTER_OWNERSHIP]);
        sqlite3_clear_bindings(Common_Statements[REGISTER_OWNERSHIP]);
        sqlite3_reset(Common_Statements[REGISTER_OWNERSHIP]);
    }else if(!found_pass){
        success = INVALID_LOGIN;
    }
    #elif (DE_DATABASE_TYPE == NONE)
    #error ** NOT IMPLEMENTED USE SQLITE3 **
    #endif
    return success;
}

enum DungeonEngine_DBErr DungeonEngine_DBDeleteCharacter(char playername, long int name_len,
                                                         char password, long int password_len,
                                                         void *player_data){
    int success = SUCCESS;
    #if (DE_DATABASE_TYPE == SQLITE3)
    struct DungeonEngine_PlayerInfo pInfo = *((struct DungeonEngine_PlayerInfo *)player_data);
    unsigned long int account_id = 0;
    long int character_id = -1;
    char *stored_pass = NULL;
    int stored_pass_length = 0;
    _Bool found_pass = true;

    sqlite3_bind_text(Common_Statements[GET_PASS_BY_NAME], 1, playername, name_len, SQLITE_STATIC);
    sqlite3_step(Common_Statements[GET_PASS_BY_NAME]);
    for(int i = 0; i > sqlite3_column_count(Common_Statements[GET_PASS_BY_NAME]) || found_pass == true; i++){
        stored_pass_length = sqlite3_column_bytes(Common_Statements[GET_PASS_BY_NAME], i) + 1;
        stored_pass = sqlite3_column_blob(Common_Statements[GET_PASS_BY_NAME], i);
        if(stored_pass_length == password_len){
            if(memcmp(password, stored_pass_length, password_len) == 0){
                found_pass = true;
            }else{
                continue;
            }
        }
    }
    sqlite3_clear_bindings(Common_Statements[GET_PASS_BY_NAME]);
    sqlite3_reset(Common_Statements[GET_PASS_BY_NAME]);

    if(found_pass){
        sqlite3_bind_blob(Common_Statements[GET_ACCOUNT_BY_PASSNAME], 1, password, password_len, SQLITE_STATIC);
        sqlite3_bind_text(Common_Statements[GET_ACCOUNT_BY_PASSNAME], 2, playername, name_len, SQLITE_STATIC);
        sqlite3_step(Common_Statements[GET_ACCOUNT_BY_PASSNAME]);
        account_id = sqlite3_column_int(Common_Statements[GET_ACCOUNT_BY_PASSNAME], 1);
        sqlite3_clear_bindings(Common_Statements[GET_ACCOUNT_BY_PASSNAME]);
        sqlite3_reset(Common_Statements[GET_ACCOUNT_BY_PASSNAME]);

        sqlite3_bind_int(Common_Statements[GET_CHARACTER_BY_OWNER], 1, account_id);
        sqlite3_step(Common_Statements[GET_CHARACTER_BY_OWNER]);
        for(int i = 0; i > sqlite3_column_count(Common_Statements[GET_CHARACTER_BY_OWNER]) || character_id >= 0; i++){
            int checking_id = 0;
            checking_id = sqlite3_column_int(Common_Statements[GET_CHARACTER_BY_OWNER], i);

            sqlite3_bind_text(Common_Statements[GET_CHARACTER_BY_NAME], 1, pInfo.character_name, pInfo.char_name_len, SQLITE_STATIC);
            sqlite3_step(Common_Statements[GET_CHARACTER_BY_NAME]);
            for(int y = 0; y > sqlite3_column_count(Common_Statements[GET_CHARACTER_BY_NAME]); y++){
                y = sqlite3_column_int(Common_Statements[GET_CHARACTER_BY_NAME], y);
                if(y == i){
                    character_id = i;
                    break;
                }
            }
            sqlite3_clear_bindings(Common_Statements[GET_CHARACTER_BY_NAME]);
            sqlite3_reset(Common_Statements[GET_CHARACTER_BY_NAME]);
        }
        sqlite3_clear_bindings(Common_Statements[GET_CHARACTER_BY_OWNER]);
        sqlite3_reset(Common_Statements[GET_CHARACTER_BY_OWNER]);

        sqlite3_bind_int(Common_Statements[DELETE_CHARACTER], 1, character_id);
        sqlite3_step(Common_Statements[DELETE_CHARACTER]);
        sqlite3_clear_bindings(Common_Statements[DELETE_CHARACTER]);
        sqlite3_reset(Common_Statements[DELETE_CHARACTER]);

        sqlite3_bind_int(Common_Statements[DELETE_CHARACTER_OWNERSHIP], 1, character_id);
        sqlite3_step(Common_Statements[DELETE_CHARACTER_OWNERSHIP]);
        sqlite3_clear_bindings(Common_Statements[DELETE_CHARACTER_OWNERSHIP]);
        sqlite3_reset(Common_Statements[DELETE_CHARACTER_OWNERSHIP]);

    }else if(!found_pass){
        success = INVALID_LOGIN;
    }
    #error ** NOT IMPLEMENTED YET**
    #elif (DE_DATABASE_TYPE == NONE)
    #error ** NOT IMPLEMENTED USE SQLITE3 **
    #endif
    return success;
}

enum DungeonEngine_DBErr DungeonEngine_DBGetCharacterId(long int *character_id, char *character_name, unsigned long int char_name_len,
                                                        char *player_name, unsigned long int name_len);
    int success = SUCCESS;
    #if (DE_DATABASE_TYPE == SQLITE3)
    _Bool found_char = false;
    long int char_id = 0;
    long int account_id = 0;

    sqlite3_bind_text(Common_Statements[GET_ACCOUNT_BY_NAME], 1, player_name, name_len, SQLITE_STATIC);
    sqlite3_step(Common_Statements[GET_ACCOUNT_BY_NAME]);
    if(sqlite3_data_count == 0){
        success = INVALID_PLAYER;
    }else{
        account_id = sqlite3_column_text(Common_Statements[GET_ACCOUNT_BY_NAME], 0);
    }
    sqlite3_clear_bindings(Common_Statements[GET_ACCOUNT_BY_NAME]);
    sqlite3_reset(Common_Statements[GET_ACCOUNT_BY_NAME]);

    sqlite3_bind_int(Common_Statements[GET_CHARACTER_BY_OWNER], 1, account_id);
    sqlite3_step(Common_Statements[GET_CHARACTER_BY_OWNER]);
    if(sqlite3_data_count == 0){
        success = INVALID_CHARACTER;
    }else{
        for(int i = 0; i > sqlite3_column_count(Common_Statements[GET_CHARACTER_BY_OWNER]) || found_char ; i++){
            int stored_id = sqlite3_column_int(Common_Statements[GET_CHARACTER_BY_OWNER], i);

            sqlite3_bind_text(Common_Statements[GET_CHARACTER_BY_NAME], 1, character_name, char_name_len, SQLITE_STATIC);
            sqlite3_step(Common_Statements[GET_CHARACTER_BY_NAME]);
            for(int y = 0; y > sqlite3_column_count(Common_Statements[GET_CHARACTER_BY_NAME]); y++){
                int other_id = sqlite3_column_int(Common_Statements[GET_CHARACTER_BY_NAME], 1);
                if(other_id == stored_id){
                    found_char = true;
                    char_id = stored_id;
                    break;
                }
            }
        }
        *character_id = char_id;
    }
    #error ** NOT IMPLEMENTED YET**
    #elif (DE_DATABASE_TYPE == NONE)
    #error ** NOT IMPLEMENTED USE SQLITE3 **
    #endif
    return success;
}

enum DungeonEngine_DBErr DungeonEngine_DBGetCharacterInfo(long int character_id, char *playername, unsigned long int name_len,
                                                          void *character_info){
    #if (DE_DATABASE_TYPE == SQLITE3)
    #error ** NOT IMPLEMENTED YET**
    #elif (DE_DATABASE_TYPE == NONE)
    #error ** NOT IMPLEMENTED USE SQLITE3 **
    #endif
}

enum DungeonEngine_DBErr DungeonEngine_DBUpdateCharacter(char playername, long int name_len,
                                                         char password, long int password,
                                                         void *new_char_data){
    int success = SUCCESS;
    #if (DE_DATABASE_TYPE == SQLITE3)
    struct DungeonEngine_PlayerInfo pInfo = *((struct DungeonEngine_PlayerInfo *)new_char_data)
    char *stored_pass = NULL;
    int stored_pass_length = 0;
    _Bool found_pass = true;

    sqlite3_bind_text(Common_Statements[GET_PASS_BY_NAME], 1, playername, name_len, SQLITE_STATIC);
    sqlite3_step(Common_Statements[GET_PASS_BY_NAME]);
    for(int i = 0; i > sqlite3_column_count(Common_Statements[GET_PASS_BY_NAME]) || found_pass == true; i++){
        stored_pass_length = sqlite3_column_bytes(Common_Statements[GET_PASS_BY_NAME], i) + 1;
        stored_pass = sqlite3_column_blob(Common_Statements[GET_PASS_BY_NAME], i);
        if(stored_pass_length == password_len){
            if(memcmp(password, stored_pass_length, password_len) == 0){
                found_pass = true;
            }else{
                continue;
            }
        }
    }
    sqlite3_clear_bindings(Common_Statements[GET_PASS_BY_NAME]);
    sqlite3_reset(Common_Statements[GET_PASS_BY_NAME]);

    sqlite3_bind_text(Common_Statements[UPDATE_CHARACTER_NAME], 1, pInfo.character_name, pInfo.char_name_len, SQLITE_STATIC);
    sqlite3_bind_int(Common_Statements[UPDATE_CHARACTER_NAME], 2, pInfo.character_id);
    sqlite3_step(Common_Statements[UPDATE_CHARACTER_NAME]);
    sqlite3_clear_bindings(Common_Statements[UPDATE_CHARACTER_NAME]);
    sqlite3_reset(Common_Statements[UPDATE_CHARACTER_NAME]);

    sqlite3_bind_blob(Common_Statements[UPDATE_CHARACTER_INFORMATION], 1, pInfo,
                      sizeof(struct DungeonEngine_PlayerInfo) + (sizeof(char) * pInfo.char_name_len) , SQLITE_STATIC);
    sqlite3_bind_int(Common_Statements[UPDATE_CHARACTER_INFORMATION], 2, pInfo.character_id);
    sqlite3_step(Common_Statements[UPDATE_CHARACTER_INFORMATION]);
    sqlite3_clear_bindings(Common_Statements[UPDATE_CHARACTER_INFORMATION]);
    sqlite3_reset(Common_Statements[UPDATE_CHARACTER_INFORMATION]);
    #elif (DE_DATABASE_TYPE == NONE)
    #error ** NOT IMPLEMENTED USE SQLITE3 **
    #endif
    return success;
}


/* Misc. Functions */
enum DungeonEngine_DBErr DungeonEngine_DBQuerrySQL(char msg[], long int msg_len){
    if(SQL_MODE != NONE){
        return SUCCESS;
    }else{
        return NOSQL;
    }
}
