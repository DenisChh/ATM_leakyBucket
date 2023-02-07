#include <stdio.h>
#include <stdlib.h>
#include "calcul_performance.h"
#include "Graphe.h"
#include "Fenetre_principale.h"
#include "args_thread.h"

int calcul_nb_sources_connectees(Donnees_initiales di)
{
    int cmp=0;
    for (int i = 0; i < di.nb_tt_sources ; i++){
        if (di.liste_sources[i].etat){
            cmp++;
        }
    }
    return cmp;
}

float calcul_debit(int nb_pq_envoyes,int nb_pq_perdus,int diff_pqts,int nb_cycles)
{
    float debit_moyen_sortie=(float)(nb_pq_envoyes-nb_pq_perdus-diff_pqts)/(float)nb_cycles;
    return debit_moyen_sortie;
}

float calcul_taux_paquets_perdus(int nb_pq_envoyes,int nb_pq_perdus )
{
    float taux_paquets_perdus= ((float)nb_pq_perdus/(float)nb_pq_envoyes)*100;
    return taux_paquets_perdus;
}
//à voire

void calcul_performances (Donnees_initiales di, Donnees_courantes* dc,Performances* perf,int diff_pqts,int nb_cycle)
{
    dc->nb_sources_connectees=calcul_nb_sources_connectees(di);

    perf->debit_moyen=calcul_debit(dc->nb_paquets_envoyes,dc->nb_paquets_perdus,diff_pqts,nb_cycle);
    perf->taux_paquets_perdus=calcul_taux_paquets_perdus(dc->nb_paquets_envoyes,dc->nb_paquets_perdus);
}

// initialiser liste pt graphe
Liste_pt_graphe init_liste_pt_graphe(){
    Liste_pt_graphe l;

    l = malloc(sizeof(Pt_graphe));
    l->pt_nb_source_connectees.x = 0; l->pt_nb_source_connectees.y = 0;
    l->pt_nb_paquet.x = 0; l->pt_nb_paquet.y = 0;
    l->pt_nb_jeton.x = 0; l->pt_nb_jeton.y = 0;
    l->pt_debit_courant.x = 0; l->pt_debit_courant.y = 0;
    l->suivant = NULL;

    return l;
}

/* supprime tous les éléments de la liste, sauf le premier élément
 * remets les valeurs du premier élément à 0
 * renvoie la liste */
Liste_pt_graphe supprimer_liste(Liste_pt_graphe l){
    Liste_pt_graphe tmp;
    Liste_pt_graphe tmp2 = l->suivant;

    while(tmp2){
        tmp = tmp2;
        tmp2 = tmp2->suivant;
        free(tmp);
    }

    l->pt_debit_courant.x = 0; l->pt_debit_courant.y = 0;
    l->pt_nb_source_connectees.x = 0; l->pt_nb_source_connectees.y = 0;
    l->pt_nb_jeton.x = 0; l->pt_nb_jeton.y = 0;
    l->pt_nb_paquet.x = 0; l->pt_nb_paquet.y = 0;
    l->suivant = NULL;

    return l;
}

// décale tous les points de la liste vers la gauche
Liste_pt_graphe decale_abscisse_points(Liste_pt_graphe l){
    Liste_pt_graphe ltmp = l;

    while(ltmp) {
        ltmp->pt_debit_courant.x = ltmp->pt_debit_courant.x*3/4;
        ltmp->pt_nb_jeton.x = ltmp->pt_nb_jeton.x*3/4;
        ltmp->pt_nb_paquet.x = ltmp->pt_nb_paquet.x*3/4;
        ltmp->pt_nb_source_connectees.x = ltmp->pt_nb_source_connectees.x*3/4;

        ltmp = ltmp->suivant;
    }

    return l;
}

/* ajouter points aux graphes
 * paramètres : pointeur sur liste points graphe, ptr sur dernier élément de la liste, diverses variables pour les graphes
 * une fois les variables récupérées, notamment dans donnes_courantes.debit_courant, dc.nb_sources_connectees, tp_jtn.nb_elements et tp_pqts.nb_elements,
 * passer ces paramètres à la fonction ajouter_points_graphe
 * puis mettre à jour tail en faisant tail = tail->next */
Liste_pt_graphe ajouter_points_graphe(Liste_pt_graphe l, Liste_pt_graphe tail, int debit_courant, int nb_sources_connectees, int nb_jetons, int nb_paquets){
    float x;

    if(l == NULL) {
        return l;
    }
    // si le point à dessiner dépasse de la fenêtre, réduire l'écart entre les points
    if(tail->pt_debit_courant.x + intervalle_x + MARGIN_X >= MAX_X) {
        l = decale_abscisse_points(l);
        intervalle_x = intervalle_x*3/4;
    }
    // ajouter points en fin de liste
    if(tail != NULL){
        x = tail->pt_debit_courant.x + intervalle_x;
        tail->suivant = init_liste_pt_graphe();

        tail->suivant->pt_debit_courant.x = x;
        tail->suivant->pt_nb_source_connectees.x = x;
        tail->suivant->pt_nb_jeton.x = x;
        tail->suivant->pt_nb_paquet.x = x;

        tail->suivant->pt_debit_courant.y = debit_courant;
        tail->suivant->pt_nb_source_connectees.y = nb_sources_connectees;
        tail->suivant->pt_nb_jeton.y = nb_jetons;
        tail->suivant->pt_nb_paquet.y = nb_paquets;
    }

    return l;
}

