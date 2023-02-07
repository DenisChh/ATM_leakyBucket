#include <gtk/gtk.h>

#include "Arret_simulation.h"
#include "Fenetre_principale.h"
#include "RecuppererDI.h"
#include "args_thread.h"

//permet d'arreter l'ensemble de la simulation
void arret_simulation(GtkWidget* button, GdkEventButton* event, gpointer data) {
	ETAT_SYSTEME = 0;
	Param_signal2* t = (Param_signal2*)data;
	GtkWidget* button_box = t->cadre_boutons;
	GList* child_button = gtk_container_get_children(GTK_CONTAINER(button_box));
	gtk_widget_set_sensitive(g_list_nth(child_button,5)->data,TRUE);
	gtk_widget_set_sensitive(g_list_nth(child_button,2)->data,TRUE);
	gtk_widget_set_sensitive(g_list_nth(child_button,0)->data,TRUE);
	gtk_widget_set_sensitive(g_list_nth(child_button,1)->data,TRUE);
	gtk_widget_set_sensitive(g_list_nth(child_button,6)->data,FALSE);

	char label[256];

	GtkWidget* perf = t->cadre_perf;
	GList* child_perf = gtk_container_get_children(GTK_CONTAINER(perf));

	sprintf(label,"Débit moyen: %fpaquets/cycle",t->ptr_perf->debit_moyen);
	gtk_label_set_text(child_perf->data,label);
	sprintf(label,"Pourcentage de perte des paquets: %f%%",t->ptr_perf->taux_paquets_perdus);
	gtk_label_set_text(child_perf->next->data,label);
}
