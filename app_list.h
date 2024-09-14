#pragma once

#include <adwaita.h>

typedef struct
{
  GListStore *list_store;
  GtkWidget *widget;  
}
AppList;

AppList create_app_list();

void filter_app_list(GList *apps, GListStore *list_store, const char *match);
