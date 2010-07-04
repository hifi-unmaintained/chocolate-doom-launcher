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

static map_t doom1_wad_maps[] =
{
    { "1 1", "E1M1: Hangar" },
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
    { "1 1", "E1M1: Hangar" },
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
    { "2 4", "E2M4: Deimos Lab", },
    { "2 5", "E2M5: Command Center", },
    { "2 6", "E2M6: Halls of the Damned", },
    { "2 7", "E2M7: Spawning Vats", },
    { "2 8", "E2M8: Tower of Babel", },
    { "2 9", "E2M9: Fortress of Mystery", },
    { "3 1", "E3M1: Hell Keep", },
    { "3 2", "E3M2: Slough of Despair", },
    { "3 3", "E3M3: Pandemonium", },
    { "3 4", "E3M4: House of Pain", },
    { "3 5", "E3M5: Unholy Cathedral", },
    { "3 6", "E3M6: Mt. Erebus", },
    { "3 7", "E3M7: Limbo", },
    { "3 8", "E3M8: Dis", },
    { "3 9", "E3M9: Warrens", },
    { "4 1", "E4M1: Hell Beneath", },
    { "4 2", "E4M2: Perfect Hatred", },
    { "4 3", "E4M3: Sever the Wicked", },
    { "4 4", "E4M4: Unruly Evil", },
    { "4 5", "E4M5: They Will Repent", },
    { "4 6", "E4M6: Against Thee Wickedly", },
    { "4 7", "E4M7: And Hell Followed", },
    { "4 8", "E4M8: Unto the Cruel", },
    { "4 8", "E4M9: Fear", },
    { NULL, NULL }
};

static map_t doom2_wad_maps[] =
{
    { "01", "Level 1: Entryway" },
    { "02", "Level 2: Underhalls" },
    { "03", "Level 3: The Gantlet" },
    { "04", "Level 4: The Focus" },
    { "05", "Level 5: The Waste Tunnels" },
    { "06", "Level 6: The Crusher" },
    { "07", "Level 7: Dead Simple" },
    { "08", "Level 8: Tricks and Traps" },
    { "09", "Level 9: The Pit" },
    { "10", "Level 10: Refueling Base" },
    { "11", "Level 11: 'O' of Destruction!" },
    { "12", "Level 12: The Factory" },
    { "13", "Level 13: Downtown" },
    { "14", "Level 14: The Inmost Dens" },
    { "15", "Level 15: Industrial Zone" },
    { "16", "Level 16: Suburbs" },
    { "17", "Level 17: Tenements" },
    { "18", "Level 18: The Courtyard" },
    { "19", "Level 19: The Citadel" },
    { "20", "Level 20: Gotcha!" },
    { "21", "Level 21: Nirvana" },
    { "22", "Level 22: The Catacombs" },
    { "23", "Level 23: Barrels o' Fun" },
    { "24", "Level 24: The Chasm" },
    { "25", "Level 25: Bloodfalls" },
    { "26", "Level 26: The Abandoned Mines" },
    { "27", "Level 27: Monster Condo" },
    { "28", "Level 28: The Spirit World" },
    { "29", "Level 29: The Living End" },
    { "30", "Level 30: Icon of Sin" },
    { "31", "Level 31: Wolfenstein" },
    { "32", "Level 32: Grosse" },
    { NULL, NULL }
};

static map_t tnt_wad_maps[] =
{
    { NULL, NULL }
};

static map_t plutonia_wad_maps[] =
{
    { NULL, NULL }
};

static map_t chex_wad_maps[] =
{
    { NULL, NULL }
};

static iwad_t iwads[] = 
{
    { "doom.wad",     "Doom",                                IWAD_DOOM },
    { "doom2.wad",    "Doom 2",                              IWAD_DOOM2 },
    { "tnt.wad",      "Final Doom: TNT: Evilution",          IWAD_TNT },
    { "plutonia.wad", "Final Doom: The Plutonia Experiment", IWAD_PLUTONIA },
    { "doom1.wad",    "Doom shareware",                      IWAD_DOOM1 },
    { "chex.wad",     "Chex Quest",                          IWAD_CHEX },
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
    
    installed_iwads = FindInstalledIWADs();

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
    switch(GetCurrentIWAD()->mask) {
        case IWAD_DOOM:
            return doom_wad_maps[map].warp;
        case IWAD_DOOM1:
            return doom1_wad_maps[map].warp;
        case IWAD_DOOM2:
            return doom2_wad_maps[map].warp;
        case IWAD_TNT:
            return tnt_wad_maps[map].warp;
        case IWAD_PLUTONIA:
            return plutonia_wad_maps[map].warp;
        case IWAD_CHEX:
            return chex_wad_maps[map].warp;
    }
    return NULL;
}

static void AddIWADParameter(execute_context_t *exec)
{
    AddCmdLineParameter(exec, "-iwad %s", GetCurrentIWAD()->filename);
}

void L_Execute(launcher_t *data)
{
    execute_context_t *exec;

    found_iwad_selected = data->iwad;

    exec = NewExecuteContext();

    // Extra parameters come first, before all others; this way,
    // they can override any of the options set in the dialog.

    //AddExtraParameters(exec);

    AddIWADParameter(exec);
    //AddCmdLineParameter(exec, "-server");

    if(data->skill > 0)
    {
        AddCmdLineParameter(exec, "-skill %i", data->skill);
    }

    if (data->nomonsters)
    {
        AddCmdLineParameter(exec, "-nomonsters");
    }

    if (data->fast)
    {
        AddCmdLineParameter(exec, "-fast");
    }

    if (data->respawn)
    {
        AddCmdLineParameter(exec, "-respawn");
    }

    if (data->deathmatch == 1)
    {
        AddCmdLineParameter(exec, "-deathmatch");
    }
    else if (data->deathmatch == 2)
    {
        AddCmdLineParameter(exec, "-altdeath");
    }

    if (data->timer > 0)
    {
        AddCmdLineParameter(exec, "-timer %i", data->timer);
    }

    if (data->map > 0)
    {
        AddCmdLineParameter(exec, "-warp %s", MapToWarp(data->map - 1));
        printf("-warp %s\n", MapToWarp(data->map - 1));
    }

    //AddCmdLineParameter(exec, "-port %i", udpport);

    //AddWADs(exec);

    //M_SaveDefaults();
    AddConfigParameters(exec);

    ExecuteDoom(exec);

    exit(0);
}
