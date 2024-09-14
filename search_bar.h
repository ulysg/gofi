#pragma once

#include <adwaita.h>

typedef struct
{
    GListStore *list_store;
    GList *apps;
}
SearchParam;

GtkWidget *create_search_bar(SearchParam *search_param);
