#ifndef LANCER_SIMULATION_H_INCLUDED
#define LANCER_SIMULATION_H_INCLUDED

#include <gtk/gtk.h>
#include "Outils_gtk.h"
#include "Fenetre_principale.h"
#include "Graphe.h"
//#include "RecuppererDI.h"

typedef struct param_signal
{
	GtkWidget* layout;
	GtkWidget* fenetre_mere;
	GtkWidget* fenetre_source;
	GtkWidget* source;
}Param_signal;

typedef struct param_signal2{
	GtkWidget* cadre_donnees_initiales;
	GtkWidget* cadre_perf;
	GtkWidget* fenetre_source;
	GtkWidget* cadre_boutons;
	GtkWidget* main_box;
	Draw_areas* draw_areas;
	Tampon* tp_pqt;
	Tampon* tp_jt;
	Donnees_initiales* ptr_di;
	Donnees_courantes* ptr_dc;
	Performances* ptr_perf;
	Liste_pt_graphe ptr_l_pt;
	char* adresse_sauvegarde;
	Data_graph* data_g;
	int choix;
}Param_signal2;

void lancer_simulation(GtkWidget* button,GdkEventButton* event ,gpointer data) ;
#endif // LANCER_SIMULATION_INCLUDED
