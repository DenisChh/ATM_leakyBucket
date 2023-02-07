#ifndef CHARGEMENTS_H
#define CHARGEMENTS_H
#include "Fenetre_principale.h"
#include "Graphe.h"

Donnees_initiales chargement_donnees_initiales(char* chemin_fichier);
Donnees_courantes chargement_donnees_courantes(char* chemin_fichier);
Performances chargement_performances(char* chemin_fichier);
Liste_pt_graphe charger_pt_graph(Liste_pt_graphe l, char* chemin_fichier);

#endif