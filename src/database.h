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
#ifndef SDE_DB
#define SDE_DB

enum DungeonEngine_DBErr {SUCCESS, NOSQL, SQLITE_ERR, INVALID_LOGIN, INVALID_PLAYER, INVALID_CHARACTER};

/* Important Functions */
enum DungeonEngine_DBErr DungeonEngine_DBInit(void);
enum DungeonEngine_DBErr DungeonEngine_DBShutdown(void);

/* Player Functions */
enum DungeonEngine_DBErr DungeonEngine_DBRegisterPlayer(char *playername, long int name_len,
                                                        char *password, long int password_len);
enum DungeonEngine_DBErr DungeonEngine_DBLoginPlayerByName(char *playername, long int *name_len,
                                                           char *password, long int password_len);
enum DungeonEngine_DBErr DungeonEngine_DBLoginPLayerByID(long int character_id, char *password,
                                                         long int password_len);
enum DungeonEngine_DBErr DungeonEngine_DBDeletePlayer(char *playername, long int name_len,
                                                      char *password, long int password_len);
enum DungeonEngine_DBErr DungeonEngine_DBGetPlayerName(unsigned long int player_id,
                                                       char **playername, long int *name_len);
enum DungeonEngine_DBErr DungeonEngine_DBGetPlayerId(long int *player_id, char *player_name, unsigned long int name_len);
enum DungeonEngine_DBErr DungeonEngine_DBUpdatePlayer(char playername, long int name_len,
                                                      char password, long int password,
                                                      void *new_player_data);

/* Character Functions */
enum DungeonEngine_DBErr DungeonEngine_DBAddCharacter(char playername, long int name_len,
                                                      char password, long int password_len,
                                                      void *character_data);
enum DungeonEngine_DBErr DungeonEngine_DBDeleteCharacter(char playername, long int name_len,
                                                         char password, long int password_len,
                                                         void *player_data);
enum DungeonEngine_DBErr DungeonEngine_DBGetCharacterId(long int *character_id,
                                                        char *character_name, unsigned long int char_name_len,
                                                        char *player_name, unsigned long int name_len);
enum DungeonEngine_DBErr DungeonEngine_DBGetCharacterInfo(long int character_id, void *character_info);
enum DungeonEngine_DBErr DungeonEngine_DBUpdateCharacter(char playername, long int name_len,
                                                        char password, long int password,
                                                        void *new_char_data);


/* Misc. Functions */
enum DungeonEngine_DBErr DungeonEngine_DBQuerrySQL(char msg[], long int msg_len);

#endif
