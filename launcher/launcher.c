// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// Copyright(C) 2006 Simon Howard, 2010 Toni Spets
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
// 02111-1307, USA.
//

// done a lot of copying from the setup code, some of these things could be commonized
// but I don't want to interfere with rest of the source yet

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>

#include <unistd.h>

#include "doomtype.h"
#include "launcher.h"
#include "execute.h"

static iwad_t iwads[] = 
{
    { "doom.wad",     "Doom",                                IWAD_DOOM },
    { "doom2.wad",    "Doom 2",                              IWAD_DOOM2 },
    { "tnt.wad",      "Final Doom: TNT: Evilution",          IWAD_TNT },
    { "plutonia.wad", "Final Doom: The Plutonia Experiment", IWAD_PLUTONIA },
    { "doom1.wad",    "Doom shareware",                      IWAD_DOOM1 },
    { "chex.wad",     "Chex Quest",                          IWAD_CHEX },
};

static map_t doom1_wad_maps[] =
{
    { "1 1", "E1M1: Hanger" },
    { "1 2", "E1M2: Nuclear Plant" },
    { "1 3", "E1M3: Toxin Refinery" },
    { "1 4", "E1M4: Command Control" },
    { "1 5", "E1M5: Phobos Lab" },
    { "1 6", "E1M6: Central Processing" },
    { "1 7", "E1M7: Computer Station" },
    { "1 8", "E1M8: Phobos Anomaly", },
    { "1 9", "E1M9: Military Base", },
    { NULL, NULL }
};

static map_t doom_wad_maps[] =
{
    { "1 1", "E1M1: Hanger" },
    { "1 2", "E1M2: Nuclear Plant" },
    { "1 3", "E1M3: Toxin Refinery" },
    { "1 4", "E1M4: Command Control" },
    { "1 5", "E1M5: Phobos Lab" },
    { "1 6", "E1M6: Central Processing" },
    { "1 7", "E1M7: Computer Station" },
    { "1 8", "E1M8: Phobos Anomaly", },
    { "1 9", "E1M9: Military Base", },
    { "2 1", "E2M1: Deimos Anomaly", },
    { "2 2", "E2M2: Containment Area", },
    { "2 3", "E2M3: Refinery", },
    { NULL, NULL }
};

static map_t doom2_wad_maps[] =
{
    { "01", "Level 1: Entryway" },
    { NULL, NULL }
};

// Array of IWADs found to be installed

static char *found_iwads[7] = { 0 };

// Index of the currently selected IWAD

static int found_iwad_selected;

// Filename to pass to '-iwad'.

static char *skills[] = 
{
    "I'm too young to die!",
    "Hey, not too rough.",
    "Hurt me plenty.",
    "Ultra-violence",
    "NIGHTMARE!",
    NULL
};

static char *chex_skills[] = 
{
    "Easy does it",
    "Not so sticky",
    "Gobs of goo",
    "Extreme ooze",
    "SUPER SLIMEY!",
    NULL
};

// Find an IWAD from its description

static iwad_t *GetIWADForDescription(char *description)
{
    unsigned int i;

    for (i=0; i<arrlen(iwads); ++i)
    {
        if (!strcmp(iwads[i].description, description))
        {
            return &iwads[i];
        }
    }

    return NULL;
}

static iwad_t *GetCurrentIWAD(void)
{
    return GetIWADForDescription(found_iwads[found_iwad_selected]);
}

/* return a list of iwads found, will be referenced by index later */
char **L_IWADs()
{
    int installed_iwads;
    int num_iwads;
    unsigned int i;

    // Find out what WADs are installed
    
    installed_iwads = 63; //FindInstalledIWADs();

    // Build a list of the descriptions for all installed IWADs

    num_iwads = 0;

    for (i=0; i<arrlen(iwads); ++i)
    {
        if (installed_iwads & iwads[i].mask)
        {
            found_iwads[num_iwads] = iwads[i].description;
            ++num_iwads;
        }
    }

    return found_iwads;
}

char **L_Skills(int iwad_idx)
{
    iwad_t* wad = GetIWADForDescription(found_iwads[iwad_idx]);

    if(wad->mask == IWAD_CHEX)
        return chex_skills;
    return skills;
}

map_t **L_Maps(int iwad_idx)
{
    int i;
    map_t **maps = NULL;

    // clean this up, please
    switch(GetIWADForDescription(found_iwads[iwad_idx])->mask) {
        case IWAD_DOOM:
            maps = malloc(sizeof(map_t*) * arrlen(doom_wad_maps));
            for(i=0;i<arrlen(doom_wad_maps);i++)
            {
                maps[i] = &doom_wad_maps[i];
            }
            break;
        case IWAD_DOOM1:
            maps = malloc(sizeof(map_t*) * arrlen(doom1_wad_maps));
            for(i=0;i<arrlen(doom1_wad_maps);i++)
            {
                maps[i] = &doom1_wad_maps[i];
            }
            break;
        case IWAD_DOOM2:
            maps = malloc(sizeof(map_t*) * arrlen(doom2_wad_maps));
            for(i=0;i<arrlen(doom2_wad_maps);i++)
            {
                maps[i] = &doom2_wad_maps[i];
            }
            break;
        default:
            return NULL;
    }

    return maps;
}

char *MapToWarp(int map)
{
    return doom_wad_maps[map].warp;
}

void L_Execute(launcher_t *data)
{
    char buf[512];

    found_iwad_selected = data->iwad;

    /*
    printf("launch options:\n iwad: %s\n skill: %d\n map: %s\n no_monsters: %d\n fast_monsters: %d\n respawn_monsters: %d\n",
            GetCurrentIWAD()->filename,
            data->skill + 1,
            MapToWarp(data->map),
            data->no_monsters,
            data->fast_monsters,
            data->respawn_monsters
        );
    */

    snprintf(buf, 512, "chocolate-doom -iwad %s -warp %s -skill %d %s%s%s",
                GetCurrentIWAD()->filename,
                MapToWarp(data->map),
                data->skill + 1,
                (data->no_monsters ? "-nomonsters " : ""),
                (data->fast_monsters ? "-fast " : ""),
                (data->respawn_monsters ? "-respawn " : "")
        );

    printf("\n%s\n\n",buf);

    system(buf);
}
