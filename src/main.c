#include <gtk/gtk.h> // Inclusion de la bibliothèque GTK
#include "gui.h"     // Inclusion du fichier d'en-tête pour la création de l'interface

/**
 * Fonction principale de l'application.
 *
 * @param argc Nombre d'arguments de la ligne de commande.
 * @param argv Tableau des arguments de la ligne de commande.
 * @return 0 si l'exécution se termine avec succès.
 */
int main(int argc, char *argv[]) {
    // Initialiser GTK, qui prépare l'environnement GTK pour l'application
    gtk_init(&argc, &argv); 

    // Créer la fenêtre principale de l'application
    GtkWidget *main_window = create_window(); 

    

    // Afficher la fenêtre et tous ses widgets enfants
    gtk_widget_show_all(main_window); 

    // Démarrer la boucle principale de GTK, qui attend les événements
    gtk_main(); 

    // Retourner 0 pour indiquer que l'application s'est terminée avec succès
    return 0; 
}
