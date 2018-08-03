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
#ifndef SDUNGEON_ENGINE
#define SDUNGEON_ENGINE

enum DungeonEngine_EnErrors {SUCCESS, FORK_FAIL};
/* DungeonEngine_Init
 *
 * Sets up any structures required for startup
 * Args: None
 * Returns: Integer (0 if successful, > 0 if error)
 */
enum DungeonEngine_EnErrors DungeonEngine_Init(void);

/* DungeonEngine_Start
 *
 * Starts the DungeonEngine Loop
 * Args: None
 * Returns Integer (0 if successful, > 0 if error)
 */
enum DungeonEngine_EnErrors DungeonEngine_Start(void);

/* DungeonEngine_ForkStart
 *
 * Starts the DungeonEngine Loop after forking
 * Args: None
 * Returns Integer (0 if successful, > 0 if error)
 */
enum DungeonEngine_EnErrors DungeonEngine_ForkStart(void);
#endif
