#include <stdio.h>
#include <stdlib.h>

#include "args_thread.h"
#include "chargements.h"
#include "Fenetre_principale.h"
#include "Graphe.h"
#include "calcul_performance.h"

Donnees_initiales chargement_donnees_initiales(char* chemin_fichier){
	FILE* fichier;
	fichier = fopen(chemin_fichier, "r");
	if (fichier == NULL){
		perror("Erreur d'ouverture de fichier");
		exit(1);
	}
	Donnees_initiales d;
	fscanf(fichier,"%d;%d;%d;%d;\n",&(d.debit_bucket),&(d.taille_tp_pqt),&(d.taille_tp_jt),&(d.nb_tt_sources));
	Source* sources = malloc(sizeof(Source) * d.nb_tt_sources);
	for (int i = 0; i < d.nb_tt_sources; i++)
	{
		fscanf(fichier,"(%d,%d);",&(sources[i].debit),&(sources[i].etat));
	}
	fclose(fichier);
	d.liste_sources = sources;
	return d;
}

Donnees_courantes chargement_donnees_courantes(char* chemin_fichier){
	FILE* fichier;
	fichier = fopen(chemin_fichier, "r");
	if (fichier == NULL){
		perror("Erreur d'ouverture de fichier");
		exit(1);
	}
	Donnees_courantes dc;
	fscanf(fichier,"%d;%d;%d;%d;%f;",
		&dc.nb_paquets_envoyes,&dc.nb_paquets_perdus,&dc.debit_courant_sortie,&dc.nb_sources_connectees,&intervalle_x);
	fclose(fichier);
	return dc;
}

Performances chargement_performances(char* chemin_fichier){
	FILE* fichier;
	fichier = fopen(chemin_fichier, "r");
	if (fichier == NULL){
		perror("Erreur d'ouverture de fichier");
		exit(1);
	}
	Performances perf;
	fscanf(fichier,"%f;%f;",&(perf.debit_moyen),&(perf.taux_paquets_perdus));
	fclose(fichier);
	return perf;
}

Liste_pt_graphe charger_pt_graph(Liste_pt_graphe list_pt_graph, char* chemin_fichier){
	FILE* fichier;
	fichier = fopen(chemin_fichier, "r");
	if (fichier == NULL){
		perror("Erreur d'ouverture de fichier");
		exit(1);
	}

	list_pt_graph = supprimer_liste(list_pt_graph);
	Liste_pt_graphe tmp = list_pt_graph;

	while(fscanf(fichier,"({%f,%f},{%f,%f},{%f,%f},{%f,%f});",
			&(tmp->pt_debit_courant.x),&(tmp->pt_debit_courant.y),
			&(tmp->pt_nb_jeton.x),&(tmp->pt_nb_jeton.y),
			&(tmp->pt_nb_paquet.x),&(tmp->pt_nb_paquet.y),
			&(tmp->pt_nb_source_connectees.x),&(tmp->pt_nb_source_connectees.y)) == 8){
		fseek(fichier, 1, SEEK_CUR);
		if(fgetc(fichier) != EOF){
			tmp->suivant = malloc(sizeof(Pt_graphe));
			tmp = tmp->suivant;
			fseek(fichier, -1, SEEK_CUR);
		}
		tmp->suivant = NULL;
	}

	fclose(fichier);
	return list_pt_graph;
}