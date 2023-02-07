//
// Created by user on 26/04/19.
//

#include <math.h>
#include "Graphe.h"
#include "Outils_gtk.h"
#include "args_thread.h"

// dessiner les axes en abscisse et en ordonnée
void dessiner_axes_graphes(cairo_t* cr, gint width, gint height){
    cairo_move_to(cr, MARGIN_X, height - MARGIN_Y);
    cairo_line_to(cr, MARGIN_X, 0);
    cairo_move_to(cr, MARGIN_X, height - MARGIN_Y);
    cairo_line_to(cr, width, height - MARGIN_Y);
    cairo_stroke(cr);
}

// dessiner les unités du graphe
void dessiner_echelles_graphes(cairo_t* cr, int width, int height, int echelle_y){
    int position;
    int valeur;
    char s[256];

    position = INTERVALLE + MARGIN_X;
    cairo_set_font_size(cr, FONT_SIZE);

    // ECHELLE X
//    while(position + width/20 < width-200){
//        sprintf(s, "%d", position/(int)intervalle_x);
//        cairo_move_to(cr, position, height);
//        cairo_show_text(cr, s);
//        position += INTERVALLE;
//    }
    cairo_move_to(cr, width - 200, height);
    cairo_show_text(cr, "TEMPS");

    // ECHELLE Y
    position = INTERVALLE;

    if(echelle_y/10 == 0){
        echelle_y = 1;
    } else if(echelle_y%10 == 0){
        echelle_y /= 10;
    } else{
        echelle_y = echelle_y/10 + 1;
    }

    valeur = echelle_y;

    while(height - position > 20){
        cairo_move_to(cr, 0, height - position - MARGIN_Y);
        sprintf(s, "%d", valeur);
        cairo_show_text(cr, s);

        position += INTERVALLE;
        valeur += echelle_y;
    }
}

// tracer les segments du point precedent au point suivant
void dessiner_courbes(cairo_t* cr, gint width, gint height, Point precedent, Point suivant, gint echelle_y){
    Point p;

    // aller point précédent
    p.x = precedent.x + MARGIN_X;
    p.y = precedent.y;
    if(echelle_y >= 10) p.y = p.y * 10 / echelle_y * INTERVALLE;
    else p.y *= INTERVALLE;
    p.y = height - p.y - MARGIN_Y;

    cairo_move_to(cr, p.x, p.y);

    // tracer au point suivant
    p.x = suivant.x + MARGIN_X;
    p.y = suivant.y;
    if(echelle_y >= 10) p.y = p.y * 10 / echelle_y * INTERVALLE;
    else p.y *= INTERVALLE;
    p.y = height - p.y - MARGIN_Y;

    // cairo
    cairo_line_to(cr, p.x, p.y);
    cairo_move_to(cr, p.x, p.y);
    cairo_arc(cr, p.x, p.y, 2, 0, 2 * G_PI);    // dessiner un cercle aux coordonnées x, y, de rayon 2, démarrant à l'angle 0, jusqu'à l'angle 2pi

    cairo_stroke(cr);   // afficher le masque
}

// appelle les fonctions call_back pour dessiner les 4 graphes, les axes et les échelles associées
// on récupère la liste de points du graphe via la variable data
// pour savoir quel graphe a appelé la fonction dessiner_graphe, on récupère le label de la frame appelante
void dessiner_graphe(GtkWidget* widget, cairo_t* cr, gpointer data){
    Data_graph* d = (Data_graph*) data;
    Liste_pt_graphe tmp = d->l;

    Point precedent;
    int width, height, max_y_debit;
    max_y_debit = 0;
    const char* label_graphe;

    // dimensions fenetres
    width = gtk_widget_get_allocated_width(widget);
    height = gtk_widget_get_allocated_height(widget);

    // debit max
    int i;
    for(i=0; i<d->di.nb_tt_sources; i++){
        max_y_debit += d->di.liste_sources[i].debit;
    }
    if(max_y_debit > d->di.debit_bucket)
        max_y_debit = d->di.debit_bucket;

    max_y_debit *= 2;

    // cairo
    precedent.x = 0; precedent.y = 0;
    label_graphe = gtk_frame_get_label(GTK_FRAME(gtk_widget_get_parent(widget)));

    dessiner_axes_graphes(cr, width, height);

    while(tmp != NULL) {
        if (!strcmp(label_graphe, "DEBIT COURANT (paquets/s)")) {
            dessiner_courbes(cr, width, height, precedent, tmp->pt_debit_courant, max_y_debit);
            dessiner_echelles_graphes(cr, width, height, max_y_debit);
            precedent = tmp->pt_debit_courant;
        }
        if (!strcmp(label_graphe, "NOMBRE DE SOURCES CONNECTEES")) {
            dessiner_courbes(cr, width, height, precedent, tmp->pt_nb_source_connectees, d->di.nb_tt_sources);
            dessiner_echelles_graphes(cr, width, height, d->di.nb_tt_sources);
            precedent = tmp->pt_nb_source_connectees;
        }
        if (!strcmp(label_graphe, "NOMBRE DE JETONS DISPONIBLES")) {
            dessiner_courbes(cr, width, height, precedent, tmp->pt_nb_jeton, d->di.taille_tp_jt);
            dessiner_echelles_graphes(cr, width, height, d->di.taille_tp_jt);
            precedent = tmp->pt_nb_jeton;
        }
        if (!strcmp(label_graphe, "NOMBRE DE PAQUETS EN ATTENTE")) {
            dessiner_courbes(cr, width, height, precedent, tmp->pt_nb_paquet, d->di.taille_tp_pqt);
            dessiner_echelles_graphes(cr, width, height, d->di.taille_tp_pqt);
            precedent = tmp->pt_nb_paquet;
        }

        tmp = tmp->suivant;
    }
}

// initialise les fenetres des graphes
// main_box > scrolled_window > vbox > frames
void init_graphes(GtkWidget* main_box, Draw_areas* d, Data_graph* data){
    GtkWidget* frame;
    GtkWidget* scrolled_window;
    GtkWidget* vbox;

    // scrolled window
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(main_box), scrolled_window);
    gtk_widget_set_size_request(scrolled_window, WIDTH, HEIGHT);

    // vbox
    vbox = init_boite_verticale(scrolled_window);

    // frames et drawing areas
    frame = init_frame("DEBIT COURANT (paquets/s)", vbox);
    d->debit = init_drawing_area(frame);
    g_signal_connect(G_OBJECT(d->debit), "draw", G_CALLBACK(dessiner_graphe), data);

    frame = init_frame("NOMBRE DE SOURCES CONNECTEES", vbox);
    d->n_sources = init_drawing_area(frame);
    g_signal_connect(G_OBJECT(d->n_sources), "draw", G_CALLBACK(dessiner_graphe), data);

    frame = init_frame("NOMBRE DE JETONS DISPONIBLES", vbox);
    d->jeton = init_drawing_area(frame);
    g_signal_connect(G_OBJECT(d->jeton), "draw", G_CALLBACK(dessiner_graphe), data);

    frame = init_frame("NOMBRE DE PAQUETS EN ATTENTE", vbox);
    d->paquet = init_drawing_area(frame);
    g_signal_connect(G_OBJECT(d->paquet), "draw", G_CALLBACK(dessiner_graphe), data);

    gtk_widget_show_all(main_box);
}