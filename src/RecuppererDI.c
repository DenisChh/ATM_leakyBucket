#include "RecuppererDI.h"
#include "Fenetre_source.h"

Donnees_initiales recupperer_donnees_initiales(GtkWidget* cadre_donnees_initiales, GtkWidget* fenetre_sources){
	//Création de la structure stockant les entrées
	//Ainsi que la création et l'initialisation d'une structre qui sera renvoyée en cas d'échec
	Donnees_initiales di;
	Donnees_initiales fail;
	fail.liste_sources = NULL;

	//Initialisation de la liste chaînée des widgets du cadre des données initiales
	GList* children_init = gtk_container_get_children(GTK_CONTAINER(cadre_donnees_initiales));

	//Récupération et vérification du contenu des champs de saisie de la fenêtre mère
	if(sscanf(gtk_entry_get_text(GTK_ENTRY(g_list_nth(children_init,1)->data)),"%d",&di.debit_bucket) != 1 ||
	   	sscanf(gtk_entry_get_text(GTK_ENTRY(g_list_nth(children_init,3)->data)),"%d",&di.taille_tp_pqt) != 1 ||
	   	sscanf(gtk_entry_get_text(GTK_ENTRY(g_list_nth(children_init,5)->data)),"%d",&di.taille_tp_jt) != 1)
	{
		g_list_free(children_init);
		return fail;
	}
	//Initialisation de la liste chaînée des widgets du cadre des sources
	GList* child = gtk_container_get_children(GTK_CONTAINER(gtk_container_get_children(GTK_CONTAINER(
		gtk_container_get_children(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(fenetre_sources))))->data))->data));	// dialog content > scrolled_window > viewport > vbox

	di.nb_tt_sources = (int) recuperer_nombre_sources(GTK_WIDGET(child->data));
	di.liste_sources = recuperer_sources(GTK_WIDGET(child->data));
	if(!di.liste_sources) return fail;

	//Libération de la mémoire des listes chaînées initialisées avant
	g_list_free(children_init);

	//Renvoi de la structure Donnees_initiales
	return di;
}