#include "app_list.h"

static GtkWidget *create_app_row()
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);

    GtkWidget *image = gtk_image_new();
    gtk_image_set_icon_size(GTK_IMAGE(image), GTK_ICON_SIZE_LARGE);

    GtkWidget *label = gtk_label_new(NULL);

    gtk_box_append(GTK_BOX(box), image);
    gtk_box_append(GTK_BOX(box), label);

    return box;
}

static void setup_listitem_cb(GtkListItemFactory *factory, GtkListItem *list_item)
{
    GtkWidget *row = create_app_row();

    gtk_list_item_set_child(list_item, row);
}

static void bind_listitem_cb(GtkListItemFactory *factory, GtkListItem *list_item)
{
    GtkWidget *row = gtk_list_item_get_child(list_item);

    GtkWidget *image = gtk_widget_get_first_child(row);
    GtkWidget *label = gtk_widget_get_next_sibling(image);
        
    GAppInfo *app_info = gtk_list_item_get_item(list_item);

    gtk_image_set_from_gicon(GTK_IMAGE(image), g_app_info_get_icon(app_info));
    gtk_label_set_text(GTK_LABEL(label), g_app_info_get_name(app_info));
    
}

static void activate_cb(GtkListView *list, guint position, gpointer unused)
{
    GAppInfo *app_info = g_list_model_get_item(G_LIST_MODEL(gtk_list_view_get_model(list)), position);
    g_app_info_launch(app_info, NULL, NULL, NULL);
    g_object_unref(app_info);

    g_application_quit(g_application_get_default());
}

AppList create_app_list()
{
    GListStore *list_store = g_list_store_new(G_TYPE_APP_INFO);

    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_listitem_cb), NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_listitem_cb), NULL);

    GtkWidget *list = gtk_list_view_new(GTK_SELECTION_MODEL(gtk_single_selection_new(G_LIST_MODEL(list_store))), factory);
    g_signal_connect(list, "activate", G_CALLBACK(activate_cb), NULL);

    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), list);
    gtk_widget_set_vexpand(scrolled_window, TRUE);
   
    AppList app_list;
    app_list.list_store = list_store;
    app_list.widget = scrolled_window;
  
    return app_list;
}

void filter_app_list(GListStore *list_store, const char *match)
{
    g_list_store_remove_all(list_store);

    GList *apps = g_app_info_get_all();

    for (GList *app = apps; app != NULL; app = app->next)
    {
        gpointer data = app->data;

        if (!g_app_info_should_show(data))
        {
            continue;
        }

        const char *name = g_app_info_get_name(data);       

        if (strcasestr(name, match) != NULL)
        {
            g_list_store_append(list_store, data);
        }
    }
}
