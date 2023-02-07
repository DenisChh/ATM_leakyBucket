//
// Created by user on 26/04/19.
//

#ifndef INTERFACE_GRAPHIQUE_GRAPHE_H
#define INTERFACE_GRAPHIQUE_GRAPHE_H

#include <gtk/gtk.h>
#include "Fenetre_principale.h"

// FONCTIONS
void dessiner_axes_graphes(cairo_t* cr, gint width, gint height);
void dessiner_echelles_graphes(cairo_t* cr, gint width, gint height, gint MAX_Y);
void dessiner_courbes(cairo_t* cr, gint width, gint height, Point precedent, Point suivant, gint echelle_y);
void dessiner_graphe(GtkWidget* widget, cairo_t* cr, gpointer data);
void init_graphes(GtkWidget* main_box, Draw_areas* d, Data_graph* data);

#endif //INTERFACE_GRAPHIQUE_GRAPHE_H
