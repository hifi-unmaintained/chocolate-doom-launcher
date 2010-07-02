#ifndef LAUNCHER_H

typedef struct
{
    /* index numbers of drop downs starting from 0 */
    int iwad;
    int skill;
    int map;

    /* flags */
    int no_monsters;
    int fast_monsters;
    int respawn_monsters;
} launcher_t;

typedef struct
{
    char *filename;
    char *description;
    int mask;
} iwad_t;

typedef enum
{
    WARP_DOOM1,
    WARP_DOOM2,
} warptype_t;

typedef struct
{
    char *warp;
    char *name;
} map_t;

char** L_Skills();
char** L_IWADs();
map_t** L_Maps(int);
void L_Execute(launcher_t*);

#define LAUNCHER_H
#endif
