#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

#include "Fenetre_principale.h"
#include "args_thread.h"

int ETAT_SYSTEME;

Source changement_etat_source(Source s){
	srand(time(NULL)*pthread_self());
	float rng = rand()%1000;
	rng++;
	rng/=1000;
	if(s.etat){
		if(PROBA_ON_OFF >= rng) s.etat=0;
	}
	else {
		if(PROBA_OFF_ON >= rng) s.etat=1;
	}
	return s;
}

void* couplage_jeton_paquet(void* n){
	Args_couplage* args = (Args_couplage*)n;
	float temps = TMP_CYCLE;
	Tampon* pqt = args->tp_pqt;
	Tampon* jt = args->tp_jt;
	while (1){
		while (ETAT_SYSTEME == 1){
			pthread_mutex_lock(args->ptr_mutex_pqt);
			pthread_mutex_lock(args->ptr_mutex_jt);
			if (pqt->nb_elt && jt->nb_elt){
				pqt->nb_elt--;		
				jt->nb_elt--;
			}
			pthread_mutex_unlock(args->ptr_mutex_jt);
			pthread_mutex_unlock(args->ptr_mutex_pqt);
			usleep(50000);
		}
		while(ETAT_SYSTEME == 0){
			usleep(temps*1000000);
		}
		if (ETAT_SYSTEME == 2) break;
	}
	pthread_exit(NULL);
}

void* envoi_paquets(void* n){
	Args_envoi_pqts* args = (Args_envoi_pqts*)n;
	float temps = TMP_CYCLE;
	int* a = args->ptr_pqts_envoyes;
	int* b = args->ptr_pqts_perdus;
	Source* s = args->s;
	Tampon* t = args->tp_paquets;
	int wait_time = temps / (float)s->debit*1000000;
	while (1){
		while(ETAT_SYSTEME == 1 && s->etat){
			for (int i = 0; i < s->debit; i++)
			{
				pthread_mutex_lock(args->mutex_pqts_envoyes);
				*a = *a+1;
				pthread_mutex_unlock(args->mutex_pqts_envoyes);
				pthread_mutex_lock(args->mutex_pqts);
				if(t->taille > t->nb_elt){
					t->nb_elt++;
				}
				else {
					pthread_mutex_lock(args->mutex_pqts_perdus);
					*b = *b+1;
					pthread_mutex_unlock(args->mutex_pqts_perdus);
				}
				pthread_mutex_unlock(args->mutex_pqts);
				usleep(wait_time);
			}
			*s = changement_etat_source(*s);
		}
		while(ETAT_SYSTEME == 1 && !s->etat){
			usleep(temps*1000000);
			*s = changement_etat_source(*s);
		}
		while(ETAT_SYSTEME == 0){
			usleep(temps*1000000);
		}
		if (ETAT_SYSTEME == 2) break;
	}
	pthread_exit(NULL);
}

int est_vide(Tampon tp){
	return !tp.nb_elt;
}

int est_plein(Tampon tp){
	return(tp.taille == tp.nb_elt);
}

void* generation_jetons(void* n){
	Args_gen_jts* args =(Args_gen_jts*)n;
	float temps = TMP_CYCLE;
	int wait_time = temps /(float)*(args->taux_gen)*1000000;
	while (1){
		while(ETAT_SYSTEME == 1){
			pthread_mutex_lock(&(args->mutex_jts));
			if (args->tp_jetons->nb_elt < args->tp_jetons->taille)
			{
				args->tp_jetons->nb_elt++;
			}
			pthread_mutex_unlock(&(args->mutex_jts));
			usleep(wait_time);
		}
		while(ETAT_SYSTEME == 0){
			usleep(temps*1000000);
		}
		if (ETAT_SYSTEME == 2) break;
	}
	pthread_exit(NULL);
}