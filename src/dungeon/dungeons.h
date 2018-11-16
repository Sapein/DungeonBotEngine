#ifndef SDE_DUNGEONS
#define SDE_DUNGEONS
#include "room_file_parser.h"
#include "../config.h"
#include <stdint.h>

struct DungeonEngine_DungeonRoom;
struct DungeonEngine_DungeonRoom {
    unsigned long int room_id; /* Is mainly used in room loading, it is unpredictable */
    long int x; /* This will be negative if coordinates are not used */
    long int y; /* This will be negative if coordinates are not used */
    union DungeonEngine_Flags flags;
    char room_name[500];
    char short_description[1000];
    char long_description[9999];
    struct DungeonEngine_DungeonRoom *north; /* Is NULL if coordinates are used */
    struct DungeonEngine_DungeonRoom *south; /* Is NULL if coordinates are used */
    struct DungeonEngine_DungeonRoom *east; /* Is NULL if coordinates are used */
    struct DungeonEngine_DungeonRoom *west; /* Is NULL if coordinates are used */
    void *placeholder1; /* Do Not Use */
    void *placeholder2; /* Do Not Use */
};

struct DungeonEngine_DungeonTree;

union DungeonEngine_Dungeon {
    struct DungeonEngine_DungeonRoom coords[DE_GENERATION_WIDTH][DE_GENERATION_HEIGHT];
    struct DungeonEngine_DungeonTree;
};

struct DungeonEngine_DungeonTree {
    int dungeon_id; /* This should be assigned randomly, this is linked to the session ID */
    int seed; /* This is randomly generated, but is stored to allow for reproducable testing */
    struct DungeonEngine_DungeonRoom *entrance;
    struct DungeonEngine_DungeonRoom *exit;
};

union DungeonEngine_Dungeon *DungeonEngine_DungeonGenerate(struct DungeonEngine_RoomsLoaded *rooms);
void DungeonEngine_DungeonDestroy(struct DungeonEngine_Dungeon *Dungeon);
#endif
