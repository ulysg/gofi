#include <adwaita.h>
#include <gtk4-layer-shell.h>

#include "app_list.h"
#include "search_bar.h"

static gboolean key_press_cb(GtkEventControllerKey* controller, guint keyval, guint keycode, GdkModifierType state, gpointer unused)
{
    if (keyval != GDK_KEY_Escape)
    {
        return FALSE;
    }

    g_application_quit(g_application_get_default());
    return TRUE;
}

static void commit_cb(GtkIMContext *context, const char *str, gpointer data)
{
    GtkWidget *search_bar = GTK_WIDGET(data);
    gtk_window_set_focus(GTK_WINDOW(gtk_widget_get_ancestor(search_bar, GTK_TYPE_WINDOW)), search_bar);

    int position = -1;
    gtk_editable_insert_text(GTK_EDITABLE(search_bar), str, -1, &position);
    gtk_editable_set_position(GTK_EDITABLE(search_bar), -1);
}

static void add_controllers(GtkWidget *window, GtkWidget *search_bar, GtkWidget *app_list)
{
    GtkEventController *window_controller = gtk_event_controller_key_new();
    GtkEventController *search_controller = gtk_event_controller_key_new();
    GtkEventController *listview_controller = gtk_event_controller_key_new();

    gtk_widget_add_controller(window, window_controller);
    gtk_widget_add_controller(search_bar, search_controller);
    gtk_widget_add_controller(app_list, listview_controller);

    g_signal_connect(window_controller, "key-pressed", G_CALLBACK(key_press_cb), NULL);
    g_signal_connect(search_controller, "key-pressed", G_CALLBACK(key_press_cb), NULL);

    GtkIMContext *im_context = gtk_im_multicontext_new();
    gtk_event_controller_key_set_im_context(GTK_EVENT_CONTROLLER_KEY(listview_controller), im_context);
    g_signal_connect(im_context, "commit", G_CALLBACK(commit_cb), search_bar);
}

static void activate_cb(GtkApplication *app)
{
    GtkWidget *window = gtk_application_window_new(app);

    gtk_layer_init_for_window(GTK_WINDOW(window));
    gtk_layer_set_layer(GTK_WINDOW(window), GTK_LAYER_SHELL_LAYER_TOP);
    gtk_layer_set_keyboard_mode(GTK_WINDOW(window), GTK_LAYER_SHELL_KEYBOARD_MODE_EXCLUSIVE);

    // Rounded corner hack
    gtk_widget_set_opacity(window, 0.99);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider, "window { border-radius: 12px; }");

    GtkStyleContext *context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(context,(GtkStyleProvider*) provider, GTK_STYLE_PROVIDER_PRIORITY_USER);

    AppList app_list = create_app_list();

    SearchParam *search_param = malloc(sizeof(SearchParam));
    search_param->list_store = app_list.list_store;
    search_param->apps = g_app_info_get_all();

    GtkWidget *search_bar = create_search_bar(search_param);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);

    gtk_box_append(GTK_BOX(box), search_bar);
    gtk_box_append(GTK_BOX(box), app_list.widget);

    gtk_widget_set_margin_top(box, 6);
    gtk_widget_set_margin_bottom(box, 6);
    gtk_widget_set_margin_start(box, 6);
    gtk_widget_set_margin_end(box, 6);

    add_controllers(window, search_bar, app_list.widget);

    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    gtk_window_set_child(GTK_WINDOW(window), box);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char *argv[])
{
    g_autoptr(AdwApplication) app = NULL;

    app = adw_application_new("ch.ulys.gofi", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate_cb), NULL);

    return g_application_run(G_APPLICATION(app), argc, argv);
}
