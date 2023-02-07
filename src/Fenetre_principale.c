//
// Created by user on 17/04/19.
//
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "Fenetre_principale.h"
#include "Fenetre_source.h"
#include "Lancer_simulation.h"
#include "Arret_simulation.h"
#include "afficher_donnees_initiales.h"
#include "Outils_gtk.h"
#include "Graphe.h"
#include "sauvegarder_sim.h"
#include "Nouvelle_simulation.h"
#include "args_thread.h"
#include "calcul_performance.h"
#include "sauvegardes.h"
#include "chargements.h"

/* Initialise les boutons de la fenêtre principale dans une grille
 * Lie la grille à la boite verticale principale */
void init_fenetre_principale(Args_affichage* args,GtkWidget* main_box, GtkWidget* fenetre_source){
    GtkWidget* button_box;
    GtkWidget* cadre_perf;
    GtkWidget* label_debit_moyen;
    GtkWidget* label_taux_perte;
    GtkWidget* bouton_lancer_sim;
    GtkWidget* bouton_stoper_sim;
    GtkWidget* bouton_nvl_sim;
    GtkWidget* bouton_svg_sim;
    GtkWidget* bouton_charger_sim;
    GtkWidget* bouton_charger_param;
    GtkWidget* bouton_sources;
    GtkWidget* hbox;
    GtkWidget* label;
    GtkWidget* entry_debit;
    GtkWidget* entry_jetons;
    GtkWidget* entry_paquets;

//****************************************************************************************************//
// creer boite boutons horizontale  
    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);   
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box), GTK_BUTTONBOX_CENTER);
    gtk_box_pack_start(GTK_BOX(main_box), button_box, FALSE, FALSE, 0);  // lier grille avec la boîte contenante

// Les bouton avec nom
    bouton_nvl_sim = gtk_button_new_with_label("Nouvelle simulation");  
    gtk_container_add(GTK_CONTAINER(button_box), bouton_nvl_sim);

    bouton_charger_sim = gtk_button_new_with_label("Charger simulation");
    gtk_container_add(GTK_CONTAINER(button_box), bouton_charger_sim);

    bouton_svg_sim = gtk_button_new_with_label("Sauvegarder simulation");
    gtk_container_add(GTK_CONTAINER(button_box), bouton_svg_sim);

    bouton_charger_param = gtk_button_new_with_label("Charger paramètres");
    gtk_container_add(GTK_CONTAINER(button_box), bouton_charger_param);

    bouton_sources= gtk_button_new_with_label("Sources");
    g_signal_connect(bouton_sources, "button-press-event", G_CALLBACK(afficher_fenetre_source), fenetre_source);
    gtk_container_add(GTK_CONTAINER(button_box), bouton_sources);

    bouton_lancer_sim = gtk_button_new_with_label("Lancer simulation");
    gtk_container_add(GTK_CONTAINER(button_box), bouton_lancer_sim);

    bouton_stoper_sim = gtk_button_new_with_label("Stopper simulation");
    gtk_container_add(GTK_CONTAINER(button_box), bouton_stoper_sim);

// creation d'une boite horizontale pour accueillir les widgets des DI
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,10);
    gtk_box_pack_start(GTK_BOX(main_box), hbox, FALSE, FALSE, 0);

    // DEBIT
    // label : nom de la donnée initiale
    label = gtk_label_new("DEBIT : ");
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);

    // entry : zone de saisie de texte
    entry_debit = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(hbox), entry_debit, FALSE, FALSE, 0);

    // TAMPON PAQUET
    label = gtk_label_new("CAPACITE MAX TAMPON PAQUET : ");
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
    entry_paquets = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(hbox), entry_paquets, FALSE, FALSE, 0);

    // TAMPON JETON
    label = gtk_label_new("CAPACITE MAX TAMPON JETON : ");
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
    entry_jetons = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(hbox), entry_jetons, FALSE, FALSE, 0);

    //creer cadre perf

    cadre_perf = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,100);
    gtk_box_pack_start(GTK_BOX(main_box), cadre_perf, FALSE, FALSE, 0);

    label_debit_moyen = gtk_label_new("Débit moyen: ");
    gtk_box_pack_start(GTK_BOX(cadre_perf), label_debit_moyen, FALSE, FALSE,0);

  	label_taux_perte = gtk_label_new("Taux de perte des paquets: ");
   	gtk_box_pack_start(GTK_BOX(cadre_perf), label_taux_perte, FALSE, FALSE, 50);


//Grisage initial des boutonset champs
    gtk_widget_set_sensitive(bouton_lancer_sim,FALSE);
    gtk_widget_set_sensitive(bouton_stoper_sim,FALSE);
    gtk_widget_set_sensitive(bouton_charger_param,FALSE);
    gtk_widget_set_sensitive(bouton_svg_sim,FALSE);
    gtk_widget_set_sensitive(bouton_sources,FALSE);
    gtk_widget_set_sensitive(entry_debit,FALSE);
    gtk_widget_set_sensitive(entry_jetons,FALSE);
    gtk_widget_set_sensitive(entry_paquets,FALSE);

// ********************************************LES SIGNAUX***************************************************************
    
    Param_signal2 *tt = malloc(sizeof(*tt));
    tt->cadre_donnees_initiales = hbox;
    tt->cadre_boutons = button_box;
    tt->cadre_perf = cadre_perf;
    tt->fenetre_source = fenetre_source;
    tt->main_box = main_box;
    tt->tp_pqt = args->tp_pqt;
    tt->tp_jt = args->tp_jt;
    tt->ptr_di = args->ptr_di;
    tt->ptr_dc = args->ptr_dc;
    tt->ptr_perf = args->ptr_perf;
    tt->ptr_l_pt = args->ptr_liste_pt_graphe;
    tt->adresse_sauvegarde = args->chemin_sauvegarde;
    tt->choix = 1;
    tt->data_g = malloc(sizeof(Data_graph));
    tt->draw_areas = args->draw_areas;

    Param_signal2 *tt2 = malloc(sizeof(*tt));
    *tt2 = *tt;
    tt2->choix = 2;

    g_signal_connect (bouton_lancer_sim, "button-press-event",G_CALLBACK (lancer_simulation), tt);
    g_signal_connect(bouton_charger_param, "button-press-event", G_CALLBACK(bouton_charger),tt);
    g_signal_connect(bouton_charger_sim, "button-press-event", G_CALLBACK(bouton_charger),tt2);
    g_signal_connect(bouton_svg_sim, "button-press-event", G_CALLBACK(bouton_sauvegarder), tt);
    g_signal_connect(bouton_nvl_sim, "button-press-event", G_CALLBACK(nouvelle_simulation), tt);
    g_signal_connect(bouton_stoper_sim, "button-press-event", G_CALLBACK(arret_simulation), tt);
}

// main
void* IG(void* n)
{
	Args_affichage* args = (Args_affichage*)n;
    GtkWidget* main_window;
    GtkWidget* main_box;
    GtkWidget* fenetre_sources;

    // initialisation gtk
    gtk_disable_setlocale();
    gtk_init(NULL,NULL);
    
    // creation fenetre et vbox principale
    main_window = init_fenetre("Leaky Bucket");
    main_box = init_boite_verticale(main_window);

    // fenêtre source
    fenetre_sources = init_fenetre_source();

    // boutons et entry utilisateur
    init_fenetre_principale(args, main_box, fenetre_sources);

    gtk_widget_show_all(main_window);
    gtk_main();

    args->etat_thread=0;

    ETAT_SYSTEME = 2;

    return EXIT_SUCCESS;
}
