#include <stdio.h>
#include <stdlib.h>

#include "args_thread.h"
#include "sauvegardes.h"

void sauvegarde_donnees_initiales(char* chemin, Donnees_initiales init){
	FILE* fichier;
	fichier = fopen(chemin,"w+");
	if (fichier == NULL)
	{
		printf("Erreur : création du fichier di échoué\n");
		exit(-1);
	}
	fprintf(fichier,"%d;%d;%d;%d;\n",init.debit_bucket,init.taille_tp_pqt,init.taille_tp_jt,init.nb_tt_sources);
	for (int i = 0; i < init.nb_tt_sources; i++)
	{
		fprintf(fichier,"(%d,%d);",init.liste_sources[i].debit,init.liste_sources[i].etat);
	}
	fclose(fichier);
}

void sauvegarde_donnees_courantes_graphes(char* chemin_pt_graphes, Liste_pt_graphe list_pt_graph, char* chemin_dc, Donnees_courantes dc){
	FILE* fichier_dc;
	fichier_dc = fopen(chemin_dc,"w+");
	if (fichier_dc == NULL)
	{
		printf("Erreur : création du fichier dc échoué\n");
		exit(-1);
	}

	fprintf(fichier_dc,"%d;%d;%d;%d;%f;",dc.nb_paquets_envoyes,dc.nb_paquets_perdus,dc.debit_courant_sortie,dc.nb_sources_connectees,intervalle_x);
	fclose(fichier_dc);

	FILE* fichier_pt_grph;
	fichier_pt_grph = fopen(chemin_pt_graphes,"w+");
	if (fichier_pt_grph == NULL)
	{
		printf("Erreur : création du fichier graphe échoué\n");
		exit(-1);
	}

	Liste_pt_graphe tmp = list_pt_graph;

	while(tmp){
		fprintf(fichier_pt_grph,"({%f,%f},{%f,%f},{%f,%f},{%f,%f});\n",
				tmp->pt_debit_courant.x,tmp->pt_debit_courant.y,
				tmp->pt_nb_jeton.x,tmp->pt_nb_jeton.y,
				tmp->pt_nb_paquet.x,tmp->pt_nb_paquet.y,
				tmp->pt_nb_source_connectees.x,tmp->pt_nb_source_connectees.y);
		tmp = tmp->suivant;
	}

	fclose(fichier_pt_grph);
}

void sauvegarde_performances(char* chemin, Performances perf){
	FILE* fichier;
	fichier = fopen(chemin,"w+");
	if (fichier == NULL)
	{
		printf("Erreur : création du fichier perf échoué \n");
		exit(-1);
	}
	fprintf(fichier,"%f;%f;",perf.debit_moyen,perf.taux_paquets_perdus);
	fclose(fichier);
}