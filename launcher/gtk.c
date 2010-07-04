#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "doomtype.h"
#include "m_argv.h"
#include "launcher.h"

GtkWidget *window;
GtkBuilder *builder;

#define COMBO_STATUS(a) (int)gtk_combo_box_get_active(GTK_COMBO_BOX(gtk_builder_get_object(builder, a)))
#define TOGGLE_STATUS(a) (int)gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, a)))

launcher_t *data = NULL;

void UpdateList(char **data, char *list_name, int first_empty)
{
    int i = 0;
    GtkTreeIter iter;
    GtkListStore *list_store;

    if(data == NULL)
        return;

    list_store = GTK_LIST_STORE(gtk_builder_get_object(builder, list_name));
    gtk_list_store_clear(list_store);

    if(first_empty)
    {
        gtk_list_store_append(list_store, &iter);
        gtk_list_store_set(list_store, &iter, 0, "", -1);
    }

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

    gtk_list_store_append(list_store, &iter);
    gtk_list_store_set(list_store, &iter, 0, "", -1);

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

    data->iwad = COMBO_STATUS("game_wad_file_select");
    data->skill = COMBO_STATUS("skill_level_select");
    data->map = COMBO_STATUS("level_select");
    data->nomonsters = TOGGLE_STATUS("no_monsters_check");
    data->fast = TOGGLE_STATUS("fast_monsters_check");
    data->respawn = TOGGLE_STATUS("respawn_monsters_check");

    gtk_widget_hide(window);
    gtk_widget_destroy(window);

    gtk_main_quit();

    return FALSE;
}

gint iwad_changed(GtkComboBox *widget, gpointer ptr)
{
    int iwad_idx = (int)gtk_combo_box_get_active(widget);

    // populate skill list
    int selected_skill = COMBO_STATUS("skill_level_select");
    UpdateList(L_Skills(iwad_idx), "skill_level_list", true);
    gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_builder_get_object(builder, "skill_level_select")), selected_skill);

    // populate map list 
    UpdateMapList(L_Maps(iwad_idx), "level_list");
    gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_builder_get_object(builder, "level_select")), 0);

    return FALSE;
}

gint update_advanced(GtkWidget *widget, gpointer ptr)
{
    int level = COMBO_STATUS("level_select");
    int multiplayer = TOGGLE_STATUS("multiplayer_check");

    if(level || multiplayer) {
        gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(builder, "advanced_parameters_frame")));
    } else {
        gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "advanced_parameters_frame")));
        gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "multiplayer_frame")));
        return FALSE;
    }

    if(multiplayer) {
        gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(builder, "multiplayer_frame")));
    } else {
        gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "multiplayer_frame")));
    }

    return FALSE;
}

gint update_multiplayer(GtkWidget *widget, gpointer ptr)
{
    if(TOGGLE_STATUS("multiplayer_join_radio")) {
        gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(builder, "multiplayer_join_frame")));
        gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "multiplayer_host_frame")));
    } else {
        gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(builder, "multiplayer_host_frame")));
        gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "multiplayer_join_frame")));
    }
    return FALSE;
}

gint update_autojoin(GtkWidget *widget, gpointer ptr)
{
    if(TOGGLE_STATUS("multiplayer_join_autojoin_check")) {
        gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(builder, "multiplayer_join_host_input")), FALSE);
    } else {
        gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(builder, "multiplayer_join_host_input")), TRUE);
    }
    return FALSE;
}

int main(int argc, char **argv)
{
    // required or executing will fail
    myargc = argc;
    myargv = argv;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "gtk.xml", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    gtk_builder_connect_signals(builder, NULL);

    g_signal_connect(GTK_WIDGET(gtk_builder_get_object(builder, "start_button")), "button_release_event", G_CALLBACK(click_start), NULL);
    g_signal_connect(GTK_WIDGET(gtk_builder_get_object(builder, "game_wad_file_select")), "changed", G_CALLBACK(iwad_changed), NULL);

    g_signal_connect(GTK_WIDGET(gtk_builder_get_object(builder, "level_select")), "changed", G_CALLBACK(update_advanced), NULL);
    g_signal_connect(GTK_WIDGET(gtk_builder_get_object(builder, "multiplayer_check")), "toggled", G_CALLBACK(update_advanced), NULL);

    g_signal_connect(GTK_WIDGET(gtk_builder_get_object(builder, "multiplayer_join_radio")), "toggled", G_CALLBACK(update_multiplayer), NULL);
    g_signal_connect(GTK_WIDGET(gtk_builder_get_object(builder, "multiplayer_join_autojoin_check")), "toggled", G_CALLBACK(update_autojoin), NULL);

    // populate iwad list
    UpdateList(L_IWADs(), "game_wad_file_list", false);
    gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_builder_get_object(builder, "game_wad_file_select")), 0);

    gtk_widget_show(window);

    gtk_main();

    g_object_unref (G_OBJECT (builder));

    if(data != NULL) {
        L_Execute(data);
    }

    return 0;
}
