#include "Nouvelle_simulation.h"
#include "Fenetre_principale.h"
#include "Fenetre_source.h"
#include "RecuppererDI.h"
#include "args_thread.h"
#include "calcul_performance.h"

#include <gtk/gtk.h>

#define LARGEUR	640
#define HAUTEUR	480

//permet à l’utilisateur de choisir l’emplacement de sauvegarde de sa simulation et de pouvoir
//la paramétrer pour la démarrer.
//On demande alors à l’utilisateur de choisir un chemin pour son dossier de sauvegarde et un nom de fichier.
//La simulation aura le même nom que le nom de fichier.
void nouvelle_simulation(GtkWidget* button, GdkEventButton* event, gpointer data){
	Param_signal2 *t = (Param_signal2 *)data;

	GtkWidget* button_box = t->cadre_boutons;

	GList* child_init = NULL;
	GList* child_source = NULL;
	GList* child_button = NULL;
	GList* child_source_button = NULL;
	GtkWidget* dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER;

	//On crée un dossier avec le nom de la simulation dans un dossier de sauvegarde
	dialog = gtk_file_chooser_dialog_new("Créer dossier", NULL, action, ("Annuler"),
				GTK_RESPONSE_CANCEL, ("Sélectionner"), GTK_RESPONSE_ACCEPT, NULL);

	gtk_widget_show_all(dialog);

	gint res = gtk_dialog_run(GTK_DIALOG(dialog));
	
	//On vide les champs de texte et les sources
	if (res == GTK_RESPONSE_ACCEPT){
		if (t->ptr_di->liste_sources) free(t->ptr_di->liste_sources);
		t->ptr_dc->nb_paquets_envoyes = 0;
		t->ptr_dc->nb_paquets_perdus = 0;
		t->tp_pqt->nb_elt = 0;
		t->tp_jt->nb_elt = 0;

		child_init = gtk_container_get_children(GTK_CONTAINER(t->cadre_donnees_initiales));
		child_source = gtk_container_get_children(GTK_CONTAINER(gtk_container_get_children(GTK_CONTAINER(
			gtk_container_get_children(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(t->fenetre_source))))->data))->data));
		child_button = gtk_container_get_children(GTK_CONTAINER(button_box));

    	child_source_button = gtk_container_get_children(GTK_CONTAINER(gtk_container_get_children(GTK_CONTAINER(child_source->data))->data));

		gtk_widget_set_sensitive(g_list_nth(child_source_button,0)->data,TRUE);
		gtk_widget_set_sensitive(g_list_nth(child_source_button,1)->data,TRUE);
		gtk_widget_set_sensitive(g_list_nth(child_source_button,3)->data,TRUE);

		gtk_widget_set_sensitive(g_list_nth(child_button,3)->data,TRUE);
		gtk_widget_set_sensitive(g_list_nth(child_button,4)->data,TRUE);
		gtk_widget_set_sensitive(g_list_nth(child_button,5)->data,TRUE);
		gtk_widget_set_sensitive(g_list_nth(child_button,1)->data,FALSE);
		gtk_widget_set_sensitive(g_list_nth(child_button,0)->data,FALSE);

		reinit(child_source->data);
		GtkFileChooser* chooser = GTK_FILE_CHOOSER(dialog);
		sprintf(t->adresse_sauvegarde, "%s", gtk_file_chooser_get_filename(chooser));

		char titre[256];
		GtkWidget* window = gtk_widget_get_toplevel(t->main_box);
		sprintf(titre, "Leaky Bucket - %s",t->adresse_sauvegarde);
		gtk_window_set_title(GTK_WINDOW(window),titre);

		gtk_widget_set_sensitive(g_list_nth(child_init,1)->data,TRUE);
		gtk_widget_set_sensitive(g_list_nth(child_init,3)->data,TRUE);
		gtk_widget_set_sensitive(g_list_nth(child_init,5)->data,TRUE);
		gtk_entry_set_text(GTK_ENTRY(g_list_nth(child_init,1)->data),"");
		gtk_entry_set_text(GTK_ENTRY(g_list_nth(child_init,3)->data),"");
		gtk_entry_set_text(GTK_ENTRY(g_list_nth(child_init,5)->data),"");

		t->ptr_l_pt = supprimer_liste(t->ptr_l_pt);

		GtkWidget* perf = t->cadre_perf;
		GList* child_perf = gtk_container_get_children(GTK_CONTAINER(perf));

		gtk_label_set_text(child_perf->data,"Débit moyen:");
		gtk_label_set_text(child_perf->next->data,"Pourcentage de perte des paquets:");

		if(g_list_length(gtk_container_get_children(GTK_CONTAINER(t->main_box)))>3){
			gtk_widget_destroy(GTK_WIDGET(g_list_nth(gtk_container_get_children(GTK_CONTAINER(t->main_box)),3)->data));
		}
		intervalle_x = 50;
		ETAT_SYSTEME = 2;
	}
	gtk_widget_destroy(dialog);

	if(child_button) g_list_free(child_button);
	if(child_source_button) g_list_free(child_source_button);
	if (child_init) g_list_free(child_init);
	if (child_source) g_list_free(child_source);
}
