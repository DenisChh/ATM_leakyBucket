//
// Created by user on 23/04/19.
//

#ifndef INTERFACE_GRAPHIQUE_FENETRE_SOURCE_H
#define INTERFACE_GRAPHIQUE_FENETRE_SOURCE_H

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "Outils_gtk.h"
#include "Fenetre_principale.h"

// I. WIDGETS
GtkWidget* init_fenetre_source();
void add_sources_widgets(GtkWidget* vbox);
void afficher_sources(GtkWidget* grille);
void afficher_fenetre_source(GtkWidget* button, GdkEventButton* event, gpointer data);
void ajouter_source(GtkWidget* button, GdkEventButton* event, gpointer data);
void supprimer_source(GtkWidget* button, GdkEventButton* event, gpointer data);
void reinit(GtkWidget* vbox);
void reinitialiser_sources(GtkWidget* button, GdkEventButton* event, gpointer data);
void charger_sources(GtkWidget* vbox, gint n_sources, Source* s);

// II. RECUPERER DONNEES DES SOURCES
gint recuperer_nombre_sources(GtkWidget* vbox);
Source* recuperer_sources(GtkWidget* vbox);

#endif //INTERFACE_GRAPHIQUE_FENETRE_SOURCE_H
