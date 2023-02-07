#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "Fenetre_principale.h"
#include "sauvegardes.h"
#include "Outils_gtk.h"

void bouton_sauvegarder(GtkWidget* button ,GdkEventButton* event ,gpointer data) {
	Param_signal2 *t = (Param_signal2 *) data;
	char chemin[256];
	char chemin2[256];
	sprintf(chemin, "%s", t->adresse_sauvegarde);
	strcat(chemin, "/pt_graphe.csv");
	sprintf(chemin2, "%s", t->adresse_sauvegarde);
	strcat(chemin2, "/dc.csv");
	sauvegarde_donnees_courantes_graphes(chemin, t->ptr_l_pt, chemin2, *(t->ptr_dc));
	sprintf(chemin, "%s", t->adresse_sauvegarde);
	strcat(chemin, "/perf.csv");
	sauvegarde_performances(chemin, *(t->ptr_perf));

	GtkWidget* dialog = gtk_message_dialog_new (NULL,
			GTK_DIALOG_MODAL,
			GTK_MESSAGE_INFO,
			GTK_BUTTONS_OK,
			"%s",
			"Simulation sauvegardée");

	gtk_widget_set_size_request(dialog, 100, 50);

	gtk_widget_show_all(dialog);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}