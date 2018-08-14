/* A Dungeon Crawling Game Engine
 * Copyright (C) 2018 Sapeint/Chanku
 *
 * See LICENSE file for more information
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <stdlib.h>
#include <string.h>
#include "players.h"
#include "../config.h"
#include "../database.h"
#if (DE_INTERNAL_HASHING == ON)
#include "../quickhash.h"
#endif

struct DungeonEngine_Player {
    struct DungeonEngine_PlayerInfo *;
    unsigned long int pass_len;
    char *password;
};

int DungeonEngine_PlayerInit(void){
    return 0;
}

int DungeonEngine_PlayerShutdown(void){
    return 0;
}

_Bool DungeonEngine_PlayerRegister(char *player_name, unsigned long int name_len, char *password, unsigned long int password_len){
    _Bool success = true;
    int return_code = 0;
    if((return_code = DungeonEngine_DBRegisterPlayer(player_name, (long int)name_len, 
                                                     password, (long int)password_len)) != SUCCESS){
        success = false;
    }
    return success;
}

_Bool DungeonEngine_PlayerDelete(char *player_name, unsigned long int name_len, char *password, unsigned long int password_len){
    _Bool success = true;
    int return_code = 0;
    if((return_code = DungeonEngine_DBDeletePlayer(player_name, (long int)name_len, 
                                                   password, (long int)password_len)) != SUCCESS){
        success = false;
    }
    return success;
}

bool DungeonEngine_PlayerLoginByName(char *player_name, unsigned long int name_len, char *password, unsigned long int password_len,
                                     ptr_PlayerToken *Token);
    _Bool success = true;
    struct DungeonEngine_Player player;
    int error_code = DungeonEngine_DBLoginPlayerByName(playername, (long int)name_len, password, (long int)password_len);
    if(error_code != SUCCESS){
        success = false;
        Token = NULL;
    }else{
        if((error_code = DungeonEngine_DBGetPlayerId(&(player.player_id), player_name, name_len)) == SUCCESS){
            player.name = player_name;
            player.name_len = name_len;
            player.pass_len = password_len;
            player.password = password;
            *Token = player;
        }else if(error_code != SUCCESS){
            Token = NULL;
            success = false;
        }
    }
    return success;
}

bool DungeonEngine_PlayerLoginByID(int player_id, char *password, unsigned long int password_len, ptr_PlayerToken *Token);
    _Bool success = true;
    struct DungeonEngine_Player player;
    int error_code = DungeonEngine_DBLoginPlayerByID(playername, (long int)name_len, password, (long int)password_len);
    if(error_code != SUCCESS){
        success = false;
        Token = NULL;
    }else{
        if((error_code = DungeonEngine_DBGetPlayerName(player_id, &(player.player_name), &(player.name_len))) == SUCCESS){
            player.player_id = player_id;
            player.pass_len = password_len;
            player.password = password;
            *Token = player;
        }else if(error_code != SUCCESS){
            Token = NULL;
            success = false;
        }
    }
    return success;
}

struct DungeonEngine_PlayerInfo DungeonEngine_PlayerGetInfo(ptr_PlayerToken Token){
    struct DungeonEngine_PlayerInfo player = { .player_id = 0, .name = '\0', .name_len = 0};
    if(Token != NULL){
        player = *((struct DungeonEngine_PlayerInfo *)Token)
    }
    return player;
}
