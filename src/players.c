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
#include "players.h"

enum DungeonEngine_PlayerClass {WARRIOR};
enum DungeonEngine_PlayerRace {HUMAN};

struct DungeonEngine_Player{
};
struct DungeonEngine_PlayerInfo {
    int character_id;
    enum DungeonEngine_PlayerClass;
    enum DungeonEngine_PlayerRace;
    int char_level;
    int char_name_len;
    void *current_room;
    void *current_dungeon;
    void *inventory;
    char character_name[];
};
typedef *DungeonEngine_Player ptr_PlayerToken

bool DungeonEngine_PlayerLogin(int character_id, long int password, ptr_PlayerToken);
bool DungeonEngine_PlayerRegister(struct DungeonEngine_PlayerInfo, long int password);
struct DungeonEngine_PlayerInfo DungeonEngine_GetPlayer(ptr_PlayerToken);

