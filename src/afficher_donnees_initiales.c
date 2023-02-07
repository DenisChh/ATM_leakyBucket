#include "afficher_donnees_initiales.h"
#include "Fenetre_principale.h"
#include "Fenetre_source.h"
#include <gtk/gtk.h>
#include <string.h>
#include "Lancer_simulation.h"
#include "chargements.h"
#include "args_thread.h"

void bouton_charger(GtkWidget* button,GdkEventButton* event ,gpointer data){
	Param_signal2 *t = (Param_signal2 *)data;

	char dossier_sauvegarde[256];
	char chooser_string[256];

	GtkWidget* dialog;
	GtkWidget* button_box = t->cadre_boutons;

	GList* child_button = NULL;
	GList* child_source_button = NULL;
	GList* child_source = NULL;
	GList* liste_sources = NULL;

	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER;

	dialog = gtk_file_chooser_dialog_new("Créer dossier",NULL, action,("Annuler"),
		GTK_RESPONSE_CANCEL,("Sélectionner"),GTK_RESPONSE_ACCEPT,NULL);

	gtk_widget_show_all(dialog);

	gint res = gtk_dialog_run(GTK_DIALOG(dialog));
	if(res == GTK_RESPONSE_ACCEPT){
		GtkFileChooser* chooser = GTK_FILE_CHOOSER(dialog);
		sprintf(chooser_string, "%s", gtk_file_chooser_get_filename(chooser));
		sprintf(dossier_sauvegarde, "%s", chooser_string);
		strcat(dossier_sauvegarde, "/di.csv");

		if(t->choix == 1){
			*(t->ptr_di)=chargement_donnees_initiales(dossier_sauvegarde);
			afficher_donnees_initiales(t->ptr_di,t->cadre_donnees_initiales,t->fenetre_source);
		}
		else{
			sprintf(t->adresse_sauvegarde, "%s", chooser_string);

			// lectures fichiers
			*(t->ptr_di)=chargement_donnees_initiales(dossier_sauvegarde);
			afficher_donnees_initiales(t->ptr_di,t->cadre_donnees_initiales,t->fenetre_source);
			strcpy(dossier_sauvegarde,t->adresse_sauvegarde);
			strcat(dossier_sauvegarde,"/dc.csv");
			*(t->ptr_dc)=chargement_donnees_courantes(dossier_sauvegarde);		
			strcpy(dossier_sauvegarde,t->adresse_sauvegarde);
			strcat(dossier_sauvegarde,"/perf.csv");
			*(t->ptr_perf)=chargement_performances(dossier_sauvegarde);
			strcpy(dossier_sauvegarde,t->adresse_sauvegarde);
			strcat(dossier_sauvegarde,"/pt_graphe.csv");
			t->ptr_l_pt=charger_pt_graph(t->ptr_l_pt,dossier_sauvegarde);

			Liste_pt_graphe tmp = t->ptr_l_pt;
			while(tmp->suivant) tmp=tmp->suivant;
			t->tp_jt->nb_elt = tmp->pt_nb_jeton.y;
			t->tp_pqt->nb_elt = tmp->pt_nb_paquet.y;

            // widgets
            child_button = gtk_container_get_children(GTK_CONTAINER(button_box));
            child_source = gtk_container_get_children(GTK_CONTAINER(gtk_container_get_children(GTK_CONTAINER(
                    gtk_container_get_children(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(t->fenetre_source))))->data))->data));
            child_source_button = gtk_container_get_children(GTK_CONTAINER(gtk_container_get_children(GTK_CONTAINER(child_source->data))->data));
            liste_sources = gtk_container_get_children(GTK_CONTAINER(child_source->data))->next;

			gtk_widget_set_sensitive(g_list_nth(child_button,4)->data,TRUE);
			gtk_widget_set_sensitive(g_list_nth(child_button,5)->data,TRUE);

			gtk_widget_set_sensitive(g_list_nth(child_source_button,0)->data,FALSE);
			gtk_widget_set_sensitive(g_list_nth(child_source_button,1)->data,FALSE);
			gtk_widget_set_sensitive(g_list_nth(child_source_button,3)->data,FALSE);

			while(liste_sources){
				gtk_widget_set_sensitive((g_list_nth(gtk_container_get_children(GTK_CONTAINER(liste_sources->data)),1)->data),FALSE);
				gtk_widget_set_sensitive((g_list_nth(gtk_container_get_children(GTK_CONTAINER(liste_sources->data)),2)->data),FALSE);
				gtk_widget_set_sensitive((g_list_nth(gtk_container_get_children(GTK_CONTAINER(liste_sources->data)),3)->data),FALSE);
				liste_sources=liste_sources->next;
			}

			char label[256];
			GtkWidget* perf = t->cadre_perf;
			GList* child_perf = gtk_container_get_children(GTK_CONTAINER(perf));

			char titre[256];
			GtkWidget* window = gtk_widget_get_toplevel(t->main_box);
			sprintf(titre, "Leaky Bucket - %s",t->adresse_sauvegarde);
			gtk_window_set_title(GTK_WINDOW(window),titre);

			sprintf(label,"Débit moyen: %fpaquets/cycle",t->ptr_perf->debit_moyen);
			gtk_label_set_text(child_perf->data,label);
			sprintf(label,"Pourcentage de perte des paquets: %f%%",t->ptr_perf->taux_paquets_perdus);
			gtk_label_set_text(child_perf->next->data,label);

			if(g_list_length(gtk_container_get_children(GTK_CONTAINER(t->main_box)))>3){
				gtk_widget_destroy(GTK_WIDGET(g_list_nth(gtk_container_get_children(GTK_CONTAINER(t->main_box)),3)->data));
			}

			t->data_g->l = t->ptr_l_pt;
			t->data_g->di = *t->ptr_di;
			init_graphes(t->main_box, t->draw_areas, t->data_g);
		}
		ETAT_SYSTEME = 2;
	}
	if(child_button) g_list_free(child_button);
	if(child_source) g_list_free(child_source);
	if(child_source_button) g_list_free(child_source_button);

	if(dialog) gtk_widget_destroy(dialog);
}

void afficher_donnees_initiales(Donnees_initiales* ptr_di, GtkWidget* cadre_donnees_initiales, GtkWidget* fenetre_source){
		char champ_debit[256];
		char champ_pqt[256];
		char champ_jt[256];

		sprintf(champ_debit,"%d",ptr_di->debit_bucket);
		sprintf(champ_pqt,"%d",ptr_di->taille_tp_jt);
		sprintf(champ_jt,"%d",ptr_di->taille_tp_pqt);

		GList* child;
		GList* child2;

		child2 = gtk_container_get_children(GTK_CONTAINER(cadre_donnees_initiales));
		gtk_entry_set_text(GTK_ENTRY(g_list_nth(child2,1)->data),champ_debit);
		gtk_entry_set_text(GTK_ENTRY(g_list_nth(child2,3)->data),champ_pqt);
		gtk_entry_set_text(GTK_ENTRY(g_list_nth(child2,5)->data),champ_jt);

	    // récupération de la vbox
	    child = gtk_container_get_children(GTK_CONTAINER(gtk_container_get_children(GTK_CONTAINER(
	    	gtk_container_get_children(GTK_CONTAINER(
	    	gtk_dialog_get_content_area(GTK_DIALOG(fenetre_source))))->data))->data));     // dialog content > scrolled window > viewport > vbox

	    // charger les sources

	    charger_sources(child->data, ptr_di->nb_tt_sources, ptr_di->liste_sources);
	    if(child) g_list_free(child);
	    if(child2) g_list_free(child2);
}