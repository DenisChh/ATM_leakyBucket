# Variables
CFLAGS = -c -g -Wall
GTK = `pkg-config --libs --cflags gtk+-3.0`

# Edition de liens
edit: Outils_gtk.o Fenetre_principale.o Fenetre_source.o Lancer_simulation.o Arret_simulation.o RecuppererDI.o afficher_donnees_initiales.o Graphe.o sauvegarder_sim.o Nouvelle_simulation.o chargements.o sauvegardes.o main.o thread_tps_sources.o calcul_performance.o
	gcc -o Fenetre Outils_gtk.o Fenetre_principale.o Fenetre_source.o Lancer_simulation.o Arret_simulation.o RecuppererDI.o afficher_donnees_initiales.o Graphe.o sauvegarder_sim.o Nouvelle_simulation.o chargements.o sauvegardes.o main.o thread_tps_sources.o calcul_performance.o $(GTK) -lpthread -lm

# Installation
install:
	sudo apt-get install libgtk-3-dev

# Exec
run: edit
	./Fenetre

# Compilation
main.o: main.c
	gcc $(GTK) $(CFLAGS) main.c -lpthread

Outils_gtk.o : Outils_gtk.c
	gcc $(GTK) $(CFLAGS) Outils_gtk.c

Fenetre_principale.o: Fenetre_principale.c
	gcc $(GTK) $(CFLAGS) Fenetre_principale.c -lpthread

Fenetre_source.o: Fenetre_source.c
	gcc $(GTK) $(CFLAGS) Fenetre_source.c

Lancer_simulation.o: Lancer_simulation.c
	gcc $(GTK) $(CFLAGS) Lancer_simulation.c

Arret_simulation.o: Arret_simulation.c
	gcc $(GTK) $(CFLAGS) Arret_simulation.c

RecuppererDI.o: RecuppererDI.c
	gcc $(GTK) $(CFLAGS) RecuppererDI.c

afficher_donnees_initiales.o: afficher_donnees_initiales.c
	gcc $(GTK) $(CFLAGS) afficher_donnees_initiales.c

Graphe.o: Graphe.c
	gcc $(GTK) $(CFLAGS) Graphe.c

sauvegarder_sim.o: sauvegarder_sim.c
	gcc $(GTK) $(CFLAGS) sauvegarder_sim.c

Nouvelle_simulation.o: Nouvelle_simulation.c
	gcc $(GTK) $(CFLAGS) Nouvelle_simulation.c

chargements.o: chargements.c
	gcc $(GTK) $(CFLAGS) chargements.c

sauvegardes.o: sauvegardes.c
	gcc $(GTK) $(CFLAGS) sauvegardes.c

thread_tps_sources.o: thread_tps_sources.c
	gcc $(GTK) $(CFLAGS) thread_tps_sources.c -lpthread -lm

calcul_performance.o: calcul_performance.c
	gcc $(GTK) $(CFLAGS) calcul_performance.c


# Clean
clean :
	rm -rf *.o

fclean: clean
	rm -rf Fenetre
