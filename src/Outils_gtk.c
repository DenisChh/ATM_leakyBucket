//
// Created by user on 23/04/19.
//

#include "Outils_gtk.h"

// FENETRES

/* Définit le titre, la taille et la position de la fenêtre principale
 * Connecte le signal de fermeture de fenêtre
 * Renvoie la fenêtre main_window */
GtkWidget* init_fenetre(const gchar* nom){
    GtkWidget* main_window;

    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), nom);
    gtk_window_set_default_size(GTK_WINDOW(main_window), WIDTH, HEIGHT);
    gtk_window_set_position (GTK_WINDOW (main_window), GTK_WIN_POS_CENTER);
    g_signal_connect(G_OBJECT(main_window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    return main_window;
}

// Initialise une fenêtre popup avec un bouton valider
GtkWidget* init_dialog(const gchar* nom){
    return gtk_dialog_new_with_buttons(nom, NULL, GTK_DIALOG_MODAL, ("_Valider"), GTK_RESPONSE_OK, NULL);
}

// BOITES
/* Initialise une boite verticale pour ranger des widgets
 * Lie la boîte au conteneur parent
 * Renvoie la boîte */
GtkWidget* init_boite_verticale(GtkWidget* parent){
    GtkWidget* vbox;

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, Y_PADDING); // nouvelle box (orientation, espace entre les éléments de la boîte)
    gtk_container_add(GTK_CONTAINER(parent), vbox);  // lie la boîte à la fenêtre principale

    return vbox;
}

/* Initialise une boite horizontale
 * Lie la boîte au conteneur parent
 * Renvoie la boite */
GtkWidget* init_boite_horizontale(GtkWidget* parent){
    GtkWidget* hbox;

    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, X_PADDING);
    gtk_container_add(GTK_CONTAINER(parent), hbox);

    return hbox;
}

/* Initialise un conteneur dans lequel on peut dessiner (les graphes notamment)
 * Lie la zone de dessin au conteneur parent
 * Renvoie la zone de dessin */
GtkWidget* init_drawing_area(GtkWidget* parent){
    GtkWidget* drawing_area;

    drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(parent), drawing_area);
    gtk_widget_set_size_request(drawing_area, WIDTH, 4*HEIGHT/5);

    return drawing_area;
}

GtkWidget* init_frame(const gchar* nom, GtkWidget* parent){
    GtkWidget* frame;

    frame = gtk_frame_new(nom);
    gtk_container_add(GTK_CONTAINER(parent), frame);

    return frame;
}
