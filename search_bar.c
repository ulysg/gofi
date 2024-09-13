#include "app_list.h"
#include "gio/gio.h"
#include "gtk/gtk.h"

static void search_changed_cb(GtkSearchEntry *entry, gpointer data)
{
    GListStore *list_store = G_LIST_STORE(data);
    filter_app_list(list_store, gtk_editable_get_text(GTK_EDITABLE(entry)));
}

static void activate_cb(GtkSearchEntry *entry, gpointer data)
{
    GListStore *list_store = G_LIST_STORE(data);
    GAppInfo *app_info = g_list_model_get_item(G_LIST_MODEL(list_store), 0);
    
    g_app_info_launch(app_info, NULL, NULL, NULL);
    g_object_unref(app_info);

    g_application_quit(g_application_get_default());
}

GtkWidget *create_search_bar(AppList *app_list)
{
    GtkWidget *entry = gtk_search_entry_new();
    g_signal_connect(entry, "search-changed", G_CALLBACK(search_changed_cb), app_list->list_store);
    g_signal_connect(entry, "activate", G_CALLBACK(activate_cb), app_list->list_store);

    return entry;
}
