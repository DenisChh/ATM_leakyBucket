#include "Fenetre_principale.h"
#include "Graphe.h"

extern int ETAT_SYSTEME;
extern float intervalle_x;

#define TMP_CYCLE 1

typedef struct args_envoi_pqts
{
	Tampon* tp_paquets;
	Source* s;
	int* ptr_pqts_envoyes;
	int* ptr_pqts_perdus;
	pthread_mutex_t* mutex_pqts_perdus;
	pthread_mutex_t* mutex_pqts_envoyes;
	pthread_mutex_t* mutex_pqts;
}Args_envoi_pqts;

typedef struct args_couplage{
	Tampon* tp_pqt;
	Tampon* tp_jt;
	pthread_mutex_t* ptr_mutex_jt;
	pthread_mutex_t* ptr_mutex_pqt;
}Args_couplage;

typedef struct args_gen_jts
{
	Tampon* tp_jetons;
	int* taux_gen;
	pthread_mutex_t mutex_jts;
}Args_gen_jts;