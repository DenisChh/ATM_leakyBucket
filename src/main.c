#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "args_thread.h"
#include "Fenetre_principale.h"
#include "thread_tps_sources.h"
#include "Graphe.h"
#include "calcul_performance.h"
#include "sauvegardes.h"

int ETAT_SYSTEME = 2;
float intervalle_x = 50;

int main(int argc, char** argv){
	Donnees_initiales di;
	di.liste_sources=NULL;
	di.nb_tt_sources = 0;
	Donnees_courantes dc;
	dc.nb_paquets_envoyes = 0;
	dc.nb_paquets_perdus = 0;
	Performances perf;
	perf.debit_moyen=0;
	perf.taux_paquets_perdus=0;
	Draw_areas draw_areas;

	draw_areas.debit = NULL;
	draw_areas.n_sources = NULL;
	draw_areas.jeton = NULL;
	draw_areas.paquet = NULL;

	int taux_gen;
	int nb_cycle = 0;
	int diff_pqts;
	int pqts_perdus_cycle;
	int pqts_envoyes_cycle;

	Tampon tp_pqt;
	tp_pqt.nb_elt = 0;
	Tampon tp_jt;
	tp_jt.nb_elt = 0;

	Liste_pt_graphe l_pt = init_liste_pt_graphe();
	Liste_pt_graphe tail = l_pt;

	char dossier_sauvegarde[256];
	char chemin_dc[256];
	char chemin_pt_graphe[256];
	char chemin_perf[256];

	pthread_mutex_t mutex_pqt = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_pqts_env = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_pqts_perdus = PTHREAD_MUTEX_INITIALIZER;

	Args_envoi_pqts* liste_args = NULL;
	pthread_t* liste_thread_envoi = NULL;

	Args_gen_jts args_gen;
	args_gen.tp_jetons = &tp_jt;
	args_gen.taux_gen = &taux_gen;
	args_gen.mutex_jts = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

	Args_affichage arg_aff;
	arg_aff.etat_thread = 1;
	arg_aff.tp_pqt = &tp_pqt;
	arg_aff.tp_jt = &tp_jt;
	arg_aff.ptr_di = &di;
	arg_aff.ptr_dc = &dc;
	arg_aff.ptr_perf=&perf;
	arg_aff.ptr_liste_pt_graphe = l_pt;
	arg_aff.ptr_tail = tail;
	arg_aff.chemin_sauvegarde = dossier_sauvegarde;
	arg_aff.draw_areas = &draw_areas;

	Args_couplage arg_cplg;
	arg_cplg.tp_pqt = &tp_pqt;
	arg_cplg.tp_jt = &tp_jt;
	arg_cplg.ptr_mutex_pqt=&mutex_pqt;
	arg_cplg.ptr_mutex_jt=&(args_gen.mutex_jts);

	pthread_t* thread_IG = malloc(sizeof(pthread_t));
	pthread_t* couplage_pq_jt = malloc(sizeof(pthread_t));
	pthread_t* gen_jt = malloc(sizeof(pthread_t));
	pthread_create(thread_IG,NULL,IG,&arg_aff);

	while(arg_aff.etat_thread){
		nb_cycle = 0;
		tail = l_pt;
		diff_pqts = 0;

        if(ETAT_SYSTEME == 1){
			strcpy(chemin_dc,dossier_sauvegarde);
			strcat(chemin_dc,"/dc.csv");
			strcpy(chemin_perf,dossier_sauvegarde);
			strcat(chemin_perf,"/perf.csv");
			strcpy(chemin_pt_graphe,dossier_sauvegarde);
			strcat(chemin_pt_graphe,"/pt_graphe.csv");
			while(tail->suivant){
				tail = tail->suivant;
				nb_cycle++;
			}
        }
		while(ETAT_SYSTEME == 1){
			ETAT_SYSTEME = 0;
			liste_args = malloc(sizeof(Args_envoi_pqts)*di.nb_tt_sources);
			liste_thread_envoi = malloc(sizeof(pthread_t)*di.nb_tt_sources);
			for (int i = 0; i < di.nb_tt_sources; ++i)
			{
				liste_args[i].tp_paquets = &tp_pqt;
				liste_args[i].s=&(di.liste_sources[i]);
				liste_args[i].ptr_pqts_envoyes=&dc.nb_paquets_envoyes;
				liste_args[i].ptr_pqts_perdus=&dc.nb_paquets_perdus;
				liste_args[i].mutex_pqts_perdus=&mutex_pqts_perdus;
				liste_args[i].mutex_pqts_envoyes=&mutex_pqts_env;
				liste_args[i].mutex_pqts=&mutex_pqt;
				pthread_create(&liste_thread_envoi[i],NULL,envoi_paquets,&liste_args[i]);
			}
			*(args_gen.taux_gen)=di.debit_bucket;
			pthread_create(gen_jt,NULL,generation_jetons,&args_gen);
			pthread_create(couplage_pq_jt,NULL,couplage_jeton_paquet,&arg_cplg);
			ETAT_SYSTEME = 1;
			while(ETAT_SYSTEME == 0 || ETAT_SYSTEME == 1){
				while(ETAT_SYSTEME == 1){
					diff_pqts = tp_pqt.nb_elt;
					pqts_envoyes_cycle = dc.nb_paquets_envoyes;
					pqts_perdus_cycle = dc.nb_paquets_perdus;
					usleep(TMP_CYCLE*1000000);
					diff_pqts = tp_pqt.nb_elt - diff_pqts;
					pqts_envoyes_cycle = dc.nb_paquets_envoyes - pqts_envoyes_cycle;
					pqts_perdus_cycle = dc.nb_paquets_perdus - pqts_perdus_cycle;
					nb_cycle++;
					dc.debit_courant_sortie=calcul_debit(pqts_envoyes_cycle,pqts_perdus_cycle,diff_pqts,1);
					dc.nb_sources_connectees=calcul_nb_sources_connectees(di);
                    l_pt = ajouter_points_graphe(l_pt, tail, dc.debit_courant_sortie, dc.nb_sources_connectees, tp_jt.nb_elt, tp_pqt.nb_elt);
                    tail = tail->suivant;
                    calcul_performances(di,&dc,&perf,tp_pqt.nb_elt,nb_cycle);

					if(ETAT_SYSTEME == 1){
						gtk_widget_queue_draw(draw_areas.debit);
						gtk_widget_queue_draw(draw_areas.n_sources);
						gtk_widget_queue_draw(draw_areas.paquet);
						gtk_widget_queue_draw(draw_areas.jeton);
					}

					sauvegarde_donnees_courantes_graphes(chemin_pt_graphe,l_pt,chemin_dc,dc);
					sauvegarde_performances(chemin_perf,perf);
				}
				while(ETAT_SYSTEME == 0){
					usleep(TMP_CYCLE*1000000);
				}
			}
		}
		if(liste_args){
			free(liste_args);
			liste_args=NULL;
		}
		if(liste_thread_envoi){
			free(liste_thread_envoi);
			liste_thread_envoi=NULL;
		}
		usleep(TMP_CYCLE*1000000);
	}

	if(di.liste_sources) free(di.liste_sources);
	if (liste_args) free(liste_args);
	if (liste_thread_envoi) free(liste_thread_envoi);
	free(thread_IG);
	free(gen_jt);
	free(couplage_pq_jt);
}
