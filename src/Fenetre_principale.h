//
// Created by user on 23/04/19.
//

#ifndef INTERFACE_GRAPHIQUE_FENETRE_PRINCIPALE_H
#define INTERFACE_GRAPHIQUE_FENETRE_PRINCIPALE_H

#include <gtk/gtk.h>

// CONSTANTES
// etat sources
#define ON 1
#define OFF 0

// espace horizontal entre les widgets
#define X_PADDING 20
#define Y_PADDING 20

// largeur hauteur de la fenêtre principale
#define WIDTH 600
#define HEIGHT 700

// nombre max de sources
#define MAX_SOURCES 100
// graphes
#define MARGIN_X 40
#define MARGIN_Y 20
#define INTERVALLE 50
#define MAX_X 1170
#define MAX_Y_DEBIT 1000
#define MAX_Y_SOURCES 100
#define MAX_Y_JETONS 1000
#define MAX_Y_PAQUETS 1000

#define PROBA_ON_OFF 0.2
#define PROBA_OFF_ON 0.8

// cairo
#define FONT_SIZE 13

// TYPES
typedef struct source{
    int debit;
    int etat;
} Source;

typedef struct tampon
{
    int taille;
    int nb_elt; 
}Tampon;

typedef struct donnees_initiales{
    int debit_bucket;
    int taille_tp_pqt;
    int taille_tp_jt;
    int nb_tt_sources;
    Source* liste_sources;
} Donnees_initiales;

typedef struct donnees_courantes {
	int nb_paquets_envoyes;
	int nb_paquets_perdus;
	int debit_courant_sortie;
	int nb_sources_connectees;
} Donnees_courantes;

typedef struct performances{
	float debit_moyen;
	float taux_paquets_perdus;
} Performances;

typedef struct point{
    float x;
    float y;
} Point;

typedef struct pt_graphe{
    Point pt_debit_courant;
    Point pt_nb_jeton;
    Point pt_nb_paquet;
    Point pt_nb_source_connectees;
    struct pt_graphe* suivant;
} Pt_graphe;

typedef Pt_graphe* Liste_pt_graphe;

typedef struct echelles
{
    float y_graphe_debit;
    float y_graphe_paquets;
    float y_graphe_jetons;
    float y_graphe_sources;
    float x_graphes;
} Echelles;

typedef struct data_graph{
    Liste_pt_graphe l;
    Donnees_initiales di;
} Data_graph;

typedef struct draw_areas{
    GtkWidget* debit;
    GtkWidget* n_sources;
    GtkWidget* jeton;
    GtkWidget* paquet;
} Draw_areas;

typedef struct Args_affichage
{
    int etat_thread;
    Tampon* tp_pqt;
    Tampon* tp_jt;
    Donnees_initiales* ptr_di;
    Donnees_courantes* ptr_dc;
    Performances* ptr_perf;
    Liste_pt_graphe ptr_liste_pt_graphe;
    Liste_pt_graphe ptr_tail;
    char* chemin_sauvegarde;
    Draw_areas* draw_areas;
}Args_affichage;

// FONCTIONS
void init_fenetre_principale(Args_affichage* args,GtkWidget* main_box, GtkWidget* fenetre_source);
void* IG(void* n);

#endif //INTERFACE_GRAPHIQUE_FENETRE_PRINCIPALE_H
