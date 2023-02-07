//
// Created by user on 23/04/19.
//

#ifndef INTERFACE_GRAPHIQUE_OUTILS_GTK_H
#define INTERFACE_GRAPHIQUE_OUTILS_GTK_H

#include <gtk/gtk.h>
#include "Fenetre_principale.h"


GtkWidget* init_fenetre(const gchar* nom);
GtkWidget* init_boite_verticale(GtkWidget* parent);
GtkWidget* init_boite_horizontale(GtkWidget* parent);
GtkWidget* init_grille();
GtkWidget* init_dialog(const gchar* nom);
GtkWidget* init_drawing_area(GtkWidget* parent);
GtkWidget* init_frame(const gchar* nom, GtkWidget* parent);


#endif //INTERFACE_GRAPHIQUE_OUTILS_GTK_H
