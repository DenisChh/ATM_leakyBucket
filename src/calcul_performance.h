#ifndef CALCUL_PERFORMANCE_H_INCLUDED
#define CALCUL_PERFORMANCE_H_INCLUDED

#include "Graphe.h"
#include "Fenetre_principale.h"

int calcul_nb_sources_connectees(Donnees_initiales di);
float calcul_debit(int nb_pq_envoyes,int nb_pq_perdus,int diff_pqts,int nb_cycles);
float calcul_taux_paquets_perdus(int nb_pq_envoyes,int nb_pq_perdus );
void calcul_performances (Donnees_initiales di,Donnees_courantes* dc,Performances* perf,int diff_pqts,int nb_cycle);

Liste_pt_graphe init_liste_pt_graphe();
Liste_pt_graphe ajouter_points_graphe(Liste_pt_graphe l, Liste_pt_graphe tail, int debit_courant,int nb_jetons,int nb_paquets,int nb_sources_connectees);
Liste_pt_graphe supprimer_liste(Liste_pt_graphe l);

#endif // CALCUL_PERFORMANCE_H_INCLUDED
