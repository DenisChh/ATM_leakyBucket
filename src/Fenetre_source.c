//
// Created by user on 23/04/19.
//

#include "Fenetre_source.h"

// I. WIDGETS

/* Créé et renvoie la fenêtre de sources
 * Organisation des conteneurs:
    * dialog
        * dialog content
            * scrolled window
                * viewport
                    * vbox
                        * button box
                            * bouton ajouter sources
                            * bouton nombre de sources à ajouter
                            * label affichant le nombre de sources total
                            * bouton réinitialiser sources
                        * plusieurs hbox sources */
GtkWidget* init_fenetre_source(){
    GtkWidget* dialog;
    GtkWidget* dialog_content;
    GtkWidget* scrolled_window;
    GtkWidget* vbox;
    GtkWidget* button_box;
    GtkWidget* bouton;
    GtkWidget* label;

    // creation dialog
    dialog = init_dialog("Sources");
    dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // creation dialog > fenetre scrollable
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(scrolled_window, 700, 700);
    gtk_box_pack_end(GTK_BOX(dialog_content), scrolled_window, FALSE, FALSE, Y_PADDING);

    // creation scrolled > vbox
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, Y_PADDING);
    gtk_container_add(GTK_CONTAINER(scrolled_window), vbox);

    // creation button_box
    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box), GTK_BUTTONBOX_CENTER);
    gtk_box_set_spacing(GTK_BOX(button_box), X_PADDING);
    gtk_container_add(GTK_CONTAINER(vbox), button_box);

    // bouton ajouter source, appelle la fonction ajouter_source à l'appui
    bouton = gtk_button_new_with_label("Ajouter Source");
    gtk_container_add(GTK_CONTAINER(button_box), bouton);
    g_signal_connect(bouton, "button-press-event", G_CALLBACK(ajouter_source), vbox);

    // bouton nombre de sources à ajouter
    bouton = gtk_spin_button_new_with_range(0, MAX_SOURCES, 1);
    gtk_container_add(GTK_CONTAINER(button_box), bouton);

    // label indiquant le nombre de sources actuel
    label = gtk_label_new("Nombre de sources : 0");
    gtk_container_add(GTK_CONTAINER(button_box), label);

    // bouton réinitialiser les sources
    bouton = gtk_button_new_with_label("Réinitialiser");
    gtk_container_add(GTK_CONTAINER(button_box), bouton);
    g_signal_connect(bouton, "button-press-event", G_CALLBACK(reinitialiser_sources), vbox);

    return dialog;
}

/* Crée des widgets pour une nouvelle source dans une hbox
 * > hbox
 *      > label debit
 *      > entry debit
 *      > bouton etat
 *      > bouton suppression */
void creer_widgets_sources(GtkWidget* vbox, gint n_sources){
    GtkWidget* hbox;
    GtkWidget* label;
    GtkWidget* bouton;
    GtkWidget* entry;
    GList* child;

    char label_n_sources[30];
    int i;

    for(i=0; i<n_sources; i++) {
        // création boite horizontal pour les widgets
        hbox = init_boite_horizontale(vbox);

        // creation label debit
        label = gtk_label_new("Débit : ");
        gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, X_PADDING);

        // creation entrée débit
        entry = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry, FALSE, FALSE, X_PADDING);

        // creation bouton switch
        bouton = gtk_switch_new();
        gtk_box_pack_start(GTK_BOX(hbox), bouton, FALSE, FALSE, X_PADDING);

        // creation bouton suppression
        bouton = gtk_button_new_with_label("Supprimer");
        gtk_box_pack_start(GTK_BOX(hbox), bouton, FALSE, FALSE, X_PADDING);
        g_signal_connect(bouton, "button-press-event", G_CALLBACK(supprimer_source), hbox);
    }

    child = gtk_container_get_children(gtk_container_get_children(GTK_CONTAINER(vbox))->data);  // vbox > button_box > bouton, bouton, label
    label = GTK_WIDGET(child->next->next->data);    // label

    // mise à jour du texte du nombre de sources
    sprintf(label_n_sources, "Nombre de sources : %d", recuperer_nombre_sources(vbox));
    gtk_label_set_text(GTK_LABEL(label), label_n_sources);
    g_list_free(child);
}

/* Bouton ajouter source
 * A l'appui du bouton, récupère le conteneur des widgets sources
 * Appelle la fonction créér widget sources qui créé le nombre de sources défini par l'utilisateur
 * Affiche la fenêtre dialog modifiée */
void ajouter_source(GtkWidget* button, GdkEventButton* event, gpointer data){
    GList* child;

    child = gtk_container_get_children(gtk_container_get_children(GTK_CONTAINER(GTK_WIDGET(data)))->data);    // vbox > button box > bouton ajouter sources
    child = child->next;    // button_box > bouton nombre de sources à ajouter

    int nb_sources = recuperer_nombre_sources(GTK_WIDGET(data));
    int nb_sources_aj = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(child->data));

    if ((nb_sources + nb_sources_aj) > 100){
        GtkWidget* dialog = gtk_message_dialog_new (NULL,
                                                    GTK_DIALOG_MODAL,
                                                    GTK_MESSAGE_INFO,
                                                    GTK_BUTTONS_OK,
                                                    "%s",
                                                    "Attention, nombre de source maximal = 100");

        gtk_widget_set_size_request(dialog, 100, 50);

        gtk_widget_show_all(dialog);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        creer_widgets_sources(GTK_WIDGET(data), 100-nb_sources);
    }
    else creer_widgets_sources(GTK_WIDGET(data), nb_sources_aj);

    g_list_free(child);
    gtk_widget_show_all(GTK_WIDGET(data));
}

// affiche la fenêtre des sources et attend que l'utilisateur ferme la fenêtre
void afficher_fenetre_source(GtkWidget* button, GdkEventButton* event, gpointer data){
    GtkWidget* dialog;
    dialog = GTK_WIDGET(data);

    // affichage
    gtk_widget_show_all(dialog);

    // boucle récursive qui attend une action utilisateur
    gtk_dialog_run(GTK_DIALOG(dialog));

    // quitter dialog
    gtk_widget_hide(dialog);
}

// supprime un widget source lorsque l'utilisateur clique sur le bouton supprimer
void supprimer_source(GtkWidget* button, GdkEventButton* event, gpointer data){
    GtkWidget* hbox;
    GtkWidget* vbox;
    GtkWidget* label;
    GList* child;
    char label_n_sources[30];

    // récupération de la boite contenant les widgets de la source à supprimer
    hbox = GTK_WIDGET(data);

    // récupération de la vbox contenante
    vbox = gtk_widget_get_parent(hbox);

    // suppression de la hbox
    gtk_widget_destroy(hbox);

    // mise à jour du nombre de sources
    child = gtk_container_get_children(GTK_CONTAINER(gtk_container_get_children(GTK_CONTAINER(vbox))->data))->next->next;   //vbox > button_box > bouton ajouter source,bouton nombre de sources à ajouter, 
                                                                                                                            //label nombre de sources, bouton réinitialiser > label nombre de sources
    label = child->data;

    sprintf(label_n_sources, "Nombre de sources : %d", recuperer_nombre_sources(vbox));
    gtk_label_set_text(GTK_LABEL(label), label_n_sources);

    gtk_widget_show_all(vbox);
    g_list_free(child);
}

void reinit(GtkWidget* vbox){
    GtkWidget* hbox;
    GList* child;

    child = gtk_container_get_children(GTK_CONTAINER(vbox));    // button box
    GList* child2 = child->next;    // hbox

    while(child2){
        hbox = child2->data;
        child2 = child2->next;  // hbox suivante
        gtk_widget_destroy(hbox);   // détruire hbox
    }
    child2 = gtk_container_get_children(GTK_CONTAINER(child->data))->next;  // bouton nombre de sources
    gtk_spin_button_set_value(child2->data,0);

    child2 = gtk_container_get_children(GTK_CONTAINER(child->data))->next->next;    // label nombre de sources
    gtk_label_set_text(GTK_LABEL(child2->data), "Nombre de sources : 0");

    if(child2) g_list_free(child2);
    if(child) g_list_free(child);
}

// supprime tous les widgets sources
void reinitialiser_sources(GtkWidget* button, GdkEventButton* event, gpointer data){
    reinit(GTK_WIDGET(data));
}

// II. Récupérer les données des sources

/* récupérer le nombre de sources
 * on parcourt la boite contenant les widgets des sources vbox
 * on compte le nombre de widgets pour avoir le nombre de sources
 * pour rappel, la vbox contient une button_box ainsi que plusieurs horizontal boxes contenant elles-mêmes des widgets pour les sources
 * en comptant le nombre de hbox, on obtient donc le nombre de sources */
gint recuperer_nombre_sources(GtkWidget* vbox){
    GList* child;
    child = gtk_container_get_children(GTK_CONTAINER(vbox));
    int a = g_list_length(child) - 1;
    g_list_free(child);
    return a;
}

// hbox source : label débit, entry débit, bouton état, bouton supprimer
Source* recuperer_sources(GtkWidget* vbox){
    GList* vbox_child;
    GList* hbox_child;
    Source* s;
    int n_sources;
    int i;

    // recuperer nombre de sources
    n_sources = recuperer_nombre_sources(vbox);
    if (!n_sources) return NULL;

    s = malloc(sizeof(Source) * n_sources);
    vbox_child = gtk_container_get_children(GTK_CONTAINER(vbox))->next;   // vbox > hbox

    for(i=0; i<n_sources; i++){
        hbox_child = gtk_container_get_children(GTK_CONTAINER(vbox_child->data))->next;   // hbox > entry débit
        if(sscanf(gtk_entry_get_text(GTK_ENTRY(hbox_child->data)),"%d",&(s[i].debit)) != 1){
            free(s);
            g_list_free(vbox_child);
            g_list_free(hbox_child);
            return NULL;
        }
        if(gtk_switch_get_active(GTK_SWITCH(hbox_child->next->data)))   // get switch active
            s[i].etat = ON;
        else
            s[i].etat = OFF;

        vbox_child = vbox_child->next;
        g_list_free(hbox_child);
    }

    if(vbox_child) g_list_free(vbox_child);
    return s;
}

// charger sources : réinitialise les sources (détruit les widgets des sources) et créé les nouveaux widgets sources
// paramètres : nombre de sources et liste de sources
// renvoie la fenêtre de sources modifiée
void charger_sources(GtkWidget* vbox, gint n_sources, Source* s){
    GList* vbox_child;
    GList* hbox_child;
    gint i;

    char entry_text[10];

    // réinitialiser les sources
    reinit(vbox);

    // créé les nouveaux widgets sources
    creer_widgets_sources(vbox, n_sources);

    // changer l'état et le débit des sources
    vbox_child = gtk_container_get_children(GTK_CONTAINER(vbox));   // vbox > button_box
    vbox_child = vbox_child->next;    // vbox > hbox source

    for(i=0; i<n_sources; i++){
        hbox_child = gtk_container_get_children(GTK_CONTAINER(vbox_child->data));   // hbox > label débit, entry débit, bouton état, bouton supprimer
        hbox_child = hbox_child->next;  // entry débit

        sprintf(entry_text, "%d", s[i].debit);    // récupération du débit
        gtk_entry_set_text(GTK_ENTRY(hbox_child->data), entry_text);   // modification de l'entry avec le débit

        hbox_child = hbox_child->next;  // bouton état
        if(s[i].etat) gtk_switch_set_active(hbox_child->data, TRUE);

        vbox_child = vbox_child->next;
   }

    if(vbox_child) g_list_free(vbox_child);
    if(hbox_child) g_list_free(hbox_child);

    gtk_widget_show_all(vbox);
}