#include <string.h>
#include <gtk/gtk.h>

#include "Lancer_simulation.h"
#include "Fenetre_principale.h"
#include "RecuppererDI.h"
#include "sauvegardes.h"
#include "args_thread.h"

void lancer_simulation(GtkWidget* button, GdkEventButton* event ,gpointer data){
	Param_signal2 *t = (Param_signal2 *)data;

	GList *child = NULL;
	GList *vbox_child = NULL;
	GList *vbox_child_tmp = NULL;
	GList *child_button = NULL;

	GtkWidget* button_box = t->cadre_boutons;

	if(ETAT_SYSTEME == 2){
		char chemin[256];
		*(t->ptr_di) = recupperer_donnees_initiales(t->cadre_donnees_initiales,t->fenetre_source);
		if(t->ptr_di->liste_sources && strcmp(t->adresse_sauvegarde, "")){
			child = gtk_container_get_children(GTK_CONTAINER(gtk_container_get_children(GTK_CONTAINER(
				gtk_container_get_children(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(t->fenetre_source))))->data))->data));	// dialog content > scrolled_window > viewport > vbox
			GtkWidget* vbox = child->data;
			vbox_child = gtk_container_get_children(GTK_CONTAINER(vbox));
			vbox_child_tmp = vbox_child->next;
			child_button = gtk_container_get_children(GTK_CONTAINER(button_box));
			
			for(int i=0;i<t->ptr_di->nb_tt_sources;i++){
				gtk_widget_set_sensitive((g_list_nth(gtk_container_get_children(GTK_CONTAINER(vbox_child_tmp->data)),1)->data),FALSE);
				gtk_widget_set_sensitive((g_list_nth(gtk_container_get_children(GTK_CONTAINER(vbox_child_tmp->data)),2)->data),FALSE);
				gtk_widget_set_sensitive((g_list_nth(gtk_container_get_children(GTK_CONTAINER(vbox_child_tmp->data)),3)->data),FALSE);
				vbox_child_tmp= vbox_child_tmp->next;
			}
			gtk_widget_set_sensitive(g_list_nth(gtk_container_get_children(GTK_CONTAINER(vbox_child->data)),0)->data,FALSE);
			gtk_widget_set_sensitive(g_list_nth(gtk_container_get_children(GTK_CONTAINER(vbox_child->data)),1)->data,FALSE);
			gtk_widget_set_sensitive(g_list_nth(gtk_container_get_children(GTK_CONTAINER(vbox_child->data)),3)->data,FALSE);
			gtk_widget_set_sensitive(g_list_nth(gtk_container_get_children(GTK_CONTAINER(t->cadre_donnees_initiales)),1)->data,FALSE);
			gtk_widget_set_sensitive(g_list_nth(gtk_container_get_children(GTK_CONTAINER(t->cadre_donnees_initiales)),3)->data,FALSE);
			gtk_widget_set_sensitive(g_list_nth(gtk_container_get_children(GTK_CONTAINER(t->cadre_donnees_initiales)),5)->data,FALSE);

			gtk_widget_set_sensitive(g_list_nth(child_button,6)->data,TRUE);
			gtk_widget_set_sensitive(g_list_nth(child_button,0)->data,FALSE);
			gtk_widget_set_sensitive(g_list_nth(child_button,1)->data,FALSE);
			gtk_widget_set_sensitive(g_list_nth(child_button,3)->data,FALSE);
			gtk_widget_set_sensitive(g_list_nth(child_button,5)->data,FALSE);

			t->tp_pqt->taille = t->ptr_di->taille_tp_pqt;
			t->tp_jt->taille = t->ptr_di->taille_tp_jt;

			strcpy(chemin, t->adresse_sauvegarde);
			strcat(chemin,"/di.csv");
			sauvegarde_donnees_initiales(chemin,*(t->ptr_di));

			// graphes
			if(!t->ptr_l_pt->suivant){
				t->data_g->l = t->ptr_l_pt;
				t->data_g->di = *t->ptr_di;
				init_graphes(t->main_box, t->draw_areas, t->data_g);
			}

			ETAT_SYSTEME = 1;
		}
	}
	else if(ETAT_SYSTEME == 0){
		child_button = gtk_container_get_children(GTK_CONTAINER(button_box));
		gtk_widget_set_sensitive(g_list_nth(child_button,5)->data,FALSE);
		gtk_widget_set_sensitive(g_list_nth(child_button,0)->data,FALSE);
		gtk_widget_set_sensitive(g_list_nth(child_button,1)->data,FALSE);
		gtk_widget_set_sensitive(g_list_nth(child_button,2)->data,FALSE);
		gtk_widget_set_sensitive(g_list_nth(child_button,6)->data,TRUE);
		ETAT_SYSTEME = 1;
	}

	if (child_button) g_list_free(child_button);
	if (vbox_child) g_list_free(vbox_child);
	if (vbox_child_tmp) g_list_free(vbox_child_tmp);
	if (child) g_list_free(child);
}