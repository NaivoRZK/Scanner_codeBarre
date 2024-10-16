#include <gtk/gtk.h>
#include "gui.h"
#include "database.h"
#include "scanner.h"

// Déclaration des widgets pour afficher le code-barres et les messages
GtkWidget *code_label;
GtkWidget *entry_code_barre;  // Ajout de l'entrée pour le code-barres
GtkWidget *message_label;      // Label pour afficher les messages

// Cette fonction est appelée lorsque le bouton "Scanner et Ajouter Matériel" est cliqué
static void on_button_click(GtkWidget *widget, gpointer data) {
    const char *code_barre = gtk_entry_get_text(GTK_ENTRY(entry_code_barre));  // Obtenir le texte de l'entrée

    // Vérifier si l'entrée est vide
    if (strlen(code_barre) == 0) {
        gtk_label_set_text(GTK_LABEL(message_label), "Veuillez entrer un code-barres."); // Afficher un message dans l'interface
        return;
    }

    // Ajouter le produit dans la base de données
    add_item("inventaire.db", code_barre);  
    gtk_label_set_text(GTK_LABEL(code_label), code_barre);  // Mettre à jour le label avec le code-barres scanné
    gtk_label_set_text(GTK_LABEL(message_label), "Produit ajouté avec succès !"); // Afficher un message dans l'interface
}

// Création de la fenêtre
GtkWidget* create_window() {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *button;

    // Création de la fenêtre principale
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "E-stocky");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 800);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE); // Empêche le redimensionnement

    // Créer une boîte verticale pour organiser les widgets
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Créer une entrée pour le code-barres
    entry_code_barre = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry_code_barre, TRUE, TRUE, 0);

    // Ajouter un bouton pour scanner et ajouter du matériel
    button = gtk_button_new_with_label("Ajouter Matériel");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_click), NULL);  
    gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 0);

    // Créer un label pour afficher le code-barres
    code_label = gtk_label_new("Aucun code-barres scanné.");
    gtk_box_pack_start(GTK_BOX(vbox), code_label, TRUE, TRUE, 0);

    // Créer un label pour afficher les messages
    message_label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox), message_label, TRUE, TRUE, 0);

    // Connexion pour fermer la fenêtre
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    return window;
}
