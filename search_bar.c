#include "search_bar.h"

#include "app_list.h"

static void search_changed_cb(GtkSearchEntry *entry, gpointer data)
{
    SearchParam *search_param = (SearchParam *) data;
    filter_app_list(search_param->apps, search_param->list_store, gtk_editable_get_text(GTK_EDITABLE(entry)));
}

static void activate_cb(GtkSearchEntry *entry, gpointer data)
{
    GListStore *list_store = G_LIST_STORE(data);
    GAppInfo *app_info = g_list_model_get_item(G_LIST_MODEL(list_store), 0);
    
    g_app_info_launch(app_info, NULL, NULL, NULL);
    g_object_unref(app_info);

    g_application_quit(g_application_get_default());
}

GtkWidget *create_search_bar(SearchParam *search_param)
{
    GtkWidget *entry = gtk_search_entry_new();
    g_signal_connect(entry, "search-changed", G_CALLBACK(search_changed_cb), search_param);
    g_signal_connect(entry, "activate", G_CALLBACK(activate_cb), search_param->list_store);

    filter_app_list(search_param->apps, search_param->list_store, "");

    return entry;
}
