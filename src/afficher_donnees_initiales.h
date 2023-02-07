#ifndef AFFICHER_DONNEES_INITIALES_H
#define AFFICHER_DONNEES_INITIALES_H
#include <gtk/gtk.h>
#include "Fenetre_principale.h"

void bouton_charger(GtkWidget* button,GdkEventButton* event ,gpointer data);
void afficher_donnees_initiales(Donnees_initiales* ptr_di, GtkWidget* cadre_donnees_initales, GtkWidget* fenetre_source);

#endif //AFFICHER_DONNEES_INITIALES_H
