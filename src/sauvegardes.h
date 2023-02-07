#ifndef SAUVEGARDES_H
#define SAUVEGARDES_H
#include "Fenetre_principale.h"
#include "Graphe.h"
#include "Lancer_simulation.h"

//Sauvegardes
void sauvegarde_donnees_initiales(char* chemin, Donnees_initiales init);
void sauvegarde_donnees_courantes_graphes(char* chemin_pt_graphes, Liste_pt_graphe list_pt_graph, char* chemin_dc, Donnees_courantes dc);
void sauvegarde_performances(char* chemin, Performances perf);

#endif