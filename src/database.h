#ifndef SDE_DB
#define SDE_DB

enum DungeonEngine_DBErr {SUCCESS, NOSQL, SQLITE_ERR, INVALID_LOGIN};

/* Important Functions */
enum DungeonEngine_DBErr DungeonEngine_DBInit(void);
enum DungeonEngine_DBErr DungeonEngine_DBShutdown(void);

/* Player Functions */
enum DungeonEngine_DBErr DungeonEngine_DBRegisterPlayer(char playername, long int name_len,
                                                        char password, long int password_len);
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
enum DungeonEngine_DBErr DungeonEngine_DBQuerrySQL(char msg[], long int msg_len);

#endif
