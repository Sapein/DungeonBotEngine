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
#ifndef SDECHARACTERS
#define SDECHARACTERS
#include <stdbool.h>
enum DungeonEngine_CharacterClass {WARRIOR};
enum DungeonEngine_CharacterRace {HUMAN};
enum DungeonEngine_CharacterAttributes {STR, CON, WIS,
                                        DEX, CHA, INT};

struct DungeonEngine_Character;
struct DungeonEngine_Inventory {
    int space_left;
    int space_used;
    void *first_item;
    void *last_item;
};

struct DungeonEngine_CharacterInfo {
    int character_id;
    enum DungeonEngine_CharacterClass char_class;
    enum DungeonEngine_CharacterRace char_race;
    int attributes[6];
    int level;
    int name_len;
    int height;
    int weight;
    void *current_room;
    void *current_dungeon;
    struct DungeonEngine_Inventory inventory;
    char *name;
};

typedef struct *DungeonEngine_Character ptr_CharacterToken;

int DungeonEngine_CharacterInit(void);
int DungeonEngine_CharacterShutdown(void);
bool DungeonEngine_CharacterLoad(int character_id, long int password, ptr_CharacterToken *character);
bool DungeonEngine_CharacterUnload(ptr_CharacterToken *character);
bool DungeonEngine_CharacterMoveRoom(ptr_CharacterToken *character, void *new_room);
void *DungeonEngine_CharacterGetRoom(ptr_CharacterToken *character);
struct DungeonEngine_CharacterInfo DungeonEngine_CharacterGetInfo(ptr_PlayerToken);
ptr_CharacterToken DungeonEngine_CharacterFromID(int character_id);
#endif
