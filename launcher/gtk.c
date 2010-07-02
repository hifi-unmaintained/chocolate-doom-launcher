#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "doomtype.h"
#include "launcher.h"

GtkWidget *window;
GtkBuilder *builder;

launcher_t *data = NULL;

void UpdateList(char **data, char *list_name)
{
    int i = 0;
    GtkTreeIter iter;
    GtkListStore *list_store;

    if(data == NULL)
        return;

    list_store = GTK_LIST_STORE(gtk_builder_get_object(builder, list_name));
    gtk_list_store_clear(list_store);

    while(data[i] != NULL)
    {
        gtk_list_store_append(list_store, &iter);
        gtk_list_store_set(list_store, &iter, 0, data[i], -1);
        i++;
    }
}

void UpdateMapList(map_t **data, char *list_name)
{
    int i = 0;
    GtkTreeIter iter;
    GtkListStore *list_store;

    list_store = GTK_LIST_STORE(gtk_builder_get_object(builder, list_name));
    gtk_list_store_clear(list_store);

    if(data == NULL)
        return;

    while(data[i]->name != NULL)
    {
        gtk_list_store_append(list_store, &iter);
        gtk_list_store_set(list_store, &iter, 0, data[i]->name, -1);
        i++;
    }
}

gint click_start(GtkWidget *widget, GdkEvent *event, gpointer ptr)
{
    data = malloc(sizeof(launcher_t));

    data->iwad = (int)gtk_combo_box_get_active(GTK_COMBO_BOX(gtk_builder_get_object(builder, "game_wad_file_select")));
    data->skill = (int)gtk_combo_box_get_active(GTK_COMBO_BOX(gtk_builder_get_object(builder, "skill_level_select")));
    data->map = (int)gtk_combo_box_get_active(GTK_COMBO_BOX(gtk_builder_get_object(builder, "level_select")));
    data->no_monsters = (int)gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "no_monsters_check")));
    data->fast_monsters = (int)gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "fast_monsters_check")));
    data->respawn_monsters = (int)gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "respawn_monsters_check")));

    gtk_widget_hide(window);
    gtk_widget_destroy(window);

    gtk_main_quit();

    return FALSE;
}

gint iwad_changed(GtkComboBox *widget, gpointer ptr)
{
    int iwad_idx = (int)gtk_combo_box_get_active(widget);

    // populate skill list
    UpdateList(L_Skills(iwad_idx), "skill_level_list");
    // select medium skill level by default
    gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_builder_get_object(builder, "skill_level_select")), 2);

    // populate map list 
    UpdateMapList(L_Maps(iwad_idx), "level_list");
    gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_builder_get_object(builder, "level_select")), 0);

    return FALSE;
}

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "gtk-minimal.xml", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    gtk_builder_connect_signals(builder, NULL);

    g_signal_connect(GTK_WIDGET(gtk_builder_get_object(builder, "start_button")), "button_release_event", G_CALLBACK(click_start), NULL);
    g_signal_connect(GTK_WIDGET(gtk_builder_get_object(builder, "game_wad_file_select")), "changed", G_CALLBACK(iwad_changed), NULL);

    // populate iwad list
    UpdateList(L_IWADs(), "game_wad_file_list");
    gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_builder_get_object(builder, "game_wad_file_select")), 0);

    gtk_widget_show(window);

    gtk_main();

    g_object_unref (G_OBJECT (builder));

    if(data != NULL) {
        L_Execute(data);
    }

    return 0;
}
