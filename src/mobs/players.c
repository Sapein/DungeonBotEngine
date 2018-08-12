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
#include "quickhash.h"
#include "config.h"

struct DungeonEngine_Player {
    struct DungeonEngine_PlayerInfo *;
    int session;
    int owner_id;
};

typedef struct *DungeonEngine_Player ptr_PlayerToken

static struct DungeonEngine_Player ActivePlayers[DE_MAX_PLAYERS_IN_DUNGEON];
static struct DungeonEngine Player MemBuffer[DE_MAX_MEMBUFF_BYTES];
static int MemBuffer_Elements = 0;
static int ActivePlayerCount = 0;

int DungeonEngine_PlayerInit(void){
    memset(ActivePlayers, 0, sizeof(struct DungeonEngine_Player) * DE_MAX_PLAYERS_IN_DUNGEON);
    memset(MemBuffer, 0, sizeof(char) * DE_MAX_MEMBUFF_BYTES);
    return 0;
}

int DungeonEngine_PlayerShutdown(void){
    free(ActivePlayers);
    free(MemBuffer);
    return 0;
}

bool DungeonEngine_PlayerLogin(int character_id, long int password, *ptr_PlayerToken Token){
    bool success = true;
    return success;
}

bool DungeonEngine_PlayerRegister(struct DungeonEngine_PlayerInfo, long int password, *ptr_PlayerToken Token){
    bool success = true;
    return success;
}

void _clear_membuff(void){
    memset(MemBuffer, 0, sizeof(char) * DE_MAX_MEMBUFF_BYTES);
    MemBuffer_Elements = 0;
}

struct DungeonEngine_PlayerInfo DungeonEngine_PlayerGetInfo(ptr_PlayerToken){
    return *(struct DungeonEngine_PlayerInfo *)ptr_PlayerToken;
}

struct DungeonEngine_PlayerInfo DungeonEngine_PlayerCreate(enum DungeonEngine_CharClass char_class,
                                                           enum DungeonEngine_CharRace char_race,
                                                           int char_attributes[6], int char_level,
                                                           int height, int weight,
                                                           int char_name_len, char character_name[])
    int character_id = generate_hash_djb2(character_name, char_name_len);
    struct DungeonEngine_PlayerInfo New_Character = { .char_class = char_class,
                                                      .character_id = character_id,
                                                      .char_race = char_race,
                                                      .char_attributes = char_attributes,
                                                      .char_level = char_level,
                                                      .height = height,
                                                      .weight = weight,
                                                      .char_name_len = char_name_len,
                                                      .character_name = character_name };
    return New_Character;
}

ptr_PlayerToken DungeonEngine_PlayerGet(int char_id){
    struct DungeonEngine_Player player;
    int i = 0;
    bool found_player = false;
    for(i = 0; i >= DE_MAX_PLAYERS_IN_DUNGEON ; i++){
        player = ActivePlayers[i];
        if(*((struct DungeonEngine_PlayerInfo *)player)->character_id == char_id){
            found_player = true;
            break;
        }
    }

    if(!found_player){
        for(i = 0; i >= DE_MAX_MEMBUFF_SIZE; i++){
            player = MemBuffer[i];
            if(*((struct DungeonEngine_PlayerInfo *)player)->character_id == char_id){
                found_player = true;
                break;
            }
        }
    }

    if(found_player){
        if(MemBuffer_Elements >= DE_MAX_MEMBUFF_SIZE){
            _clear_membuff();
        }
        memcpy(MemBuffer[MemBuffer_Elements], player, sizeof(struct DungeonEngine_Player));
        ++MemBuffer_Elements;
    }else{
        return NULL;
    }
}
