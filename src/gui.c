#include <gtk/gtk.h>
#include "gui.h"
#include "database.h"
#include "scanner.h"
#include <glib.h>


// Déclaration des widgets pour afficher le code-barres et les messages
GtkWidget *code_label;
GtkWidget *entry_code_barre;  // Ajout de l'entrée pour le code-barres
GtkWidget *message_label;      // Label pour afficher les messages comment lorqque je clqiue sur le menu capture materiel il affiche un popup pour  prendre de photo a partir webcam  j'ulmtse linux  et gstreaner
GtkWidget *menu_button;        // Bouton du menu hamburger



// Déclaration de la fonction on_menu_item_selected
static void on_menu_item_selected(GtkWidget *widget, gpointer data);
static void open_help_window(); // Déclaration de la fonction pour ouvrir la fenêtre d'aide

// Fonction pour gérer le clic sur le bouton "Scanner et Ajouter Matériel"
static void on_button_click(GtkWidget *widget, gpointer data) {
    const char *code_barre = gtk_entry_get_text(GTK_ENTRY(entry_code_barre));  // Obtenir le texte de l'entrée

    // Vérifier si l'entrée est vide
    if (strlen(code_barre) == 0) {
        gtk_label_set_text(GTK_LABEL(message_label), "Veuillez entrer un code-barres."); // Afficher un message dans l'interface
        return;
    }

    // Ajouter le produit dans la base de données
    add_item("db/inventaire.db", code_barre);  
    gtk_label_set_text(GTK_LABEL(code_label), code_barre);  // Mettre à jour le label avec le code-barres scanné
    gtk_label_set_text(GTK_LABEL(message_label), "Produit ajouté avec succès !"); // Afficher un message dans l'interface
}

// Fonction pour afficher le menu lors du clic sur le bouton hamburger
static void on_menu_button_click(GtkWidget *widget, gpointer data) {
    GtkWidget *menu, *menu_item;
    
    // Création du menu
    menu = gtk_menu_new();

    // Créer des éléments de menu
    menu_item = gtk_menu_item_new_with_label("Ajouter Matériel");
    g_signal_connect(menu_item, "activate", G_CALLBACK(on_menu_item_selected), "matériel");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    menu_item = gtk_menu_item_new_with_label("Tous les Matériels");
    g_signal_connect(menu_item, "activate", G_CALLBACK(on_menu_item_selected), "tous_materiaux");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    menu_item = gtk_menu_item_new_with_label("Aide");
    g_signal_connect(menu_item, "activate", G_CALLBACK(open_help_window), NULL); // Ouvrir la fenêtre d'aide
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    menu_item = gtk_menu_item_new_with_label("Suppression Matériel");
    g_signal_connect(menu_item, "activate", G_CALLBACK(on_menu_item_selected), "suppression");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    gtk_widget_show_all(menu); // Afficher tous les éléments du menu
    gtk_menu_popup_at_pointer(GTK_MENU(menu), NULL); // Afficher le menu à la position du pointeur
}

// Fonction pour gérer l'action de menu
static void on_menu_item_selected(GtkWidget *widget, gpointer data) {
    const char *menu_item = (const char *)data;
    if (g_strcmp0(menu_item, "matériel") == 0) {
        gtk_label_set_text(GTK_LABEL(message_label), "Gestion du Matériel sélectionnée.");
    } else if (g_strcmp0(menu_item, "tous_materiaux") == 0) {  // Gestion pour l'option "Tous les Matériels"
        gtk_label_set_text(GTK_LABEL(message_label), "Tous les Matériels sélectionnés.");
        show_all_items(); // Appel de la nouvelle fonction pour afficher tous les matériels
    } else if (g_strcmp0(menu_item, "aide") == 0) {
        gtk_label_set_text(GTK_LABEL(message_label), "Aide sélectionnée.");
    } else if (g_strcmp0(menu_item, "suppression") == 0) {
        gtk_label_set_text(GTK_LABEL(message_label), "Capturer des materiels.");
    }
}

// Fonction pour ouvrir la fenêtre d'aide
static void open_help_window() {
    GtkWidget *help_window;
    GtkWidget *help_label;
    GtkWidget *close_button;
    GtkWidget *vbox;

    // Création de la fenêtre d'aide
    help_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(help_window), "Aide");
    gtk_window_set_default_size(GTK_WINDOW(help_window), 400, 300);

    // Création d'un vbox pour contenir le label et le bouton de fermeture
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(help_window), vbox);

    // Création d'un label avec le contenu d'aide
    help_label = gtk_label_new("Bienvenue dans l'aide de l'application E-stoky !\n\n"
                               "Voici quelques conseils pour utiliser l'application :\n\n"
                               "1. Pour ajouter un matériel, entrez le code-barres et cliquez sur 'Ajout Matériel'.\n"
                               "2. Pour voir tous les matériels, utilisez le menu 'Tous les Matériels'.\n"
                               "3. Pour supprimer un matériel, entrez le code-barres et sélectionnez 'Suppression Matériel'.\n"
                               "4. Pour plus d'informations, contactez le support.\n");
    
    // Ajouter le label à la boîte
    gtk_box_pack_start(GTK_BOX(vbox), help_label, TRUE, TRUE, 0);
    
    
    // Connexion pour fermer la fenêtre
    g_signal_connect(help_window, "destroy", G_CALLBACK(gtk_widget_destroy), NULL);
    
    // Afficher la fenêtre d'aide
    gtk_widget_show_all(help_window);
}


// Création de la fenêtre
GtkWidget* create_window() {
    GtkWidget *window;
    GtkWidget *hbox;  // Boîte horizontale pour le menu et le contenu
    GtkWidget *vbox;  // Boîte verticale pour le contenu principal
    GtkWidget *menu_vbox;  // Boîte verticale pour le menu à gauche
    GtkWidget *button;

    // Création de la fenêtre principale
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "E-stoky");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 800);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE); // Empêche le redimensionnement

    // Créer une boîte horizontale pour organiser le bouton de menu et le contenu
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_add(GTK_CONTAINER(window), hbox);

    // Créer une boîte verticale pour le menu à gauche
    menu_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(hbox), menu_vbox, FALSE, FALSE, 0); // Ajouter la boîte de menu à gauche

    // Créer le bouton du menu hamburger
    menu_button = gtk_button_new_with_label("☰"); // Le symbole "☰" représente le menu hamburger
    g_signal_connect(menu_button, "clicked", G_CALLBACK(on_menu_button_click), NULL);
    gtk_box_pack_start(GTK_BOX(menu_vbox), menu_button, FALSE, FALSE, 0); // Ajouter le bouton dans la boîte de menu

    // Créer une boîte verticale pour le contenu principal
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10); // Espacement de 10 pixels
    gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, TRUE, 0); // Ajouter le vbox pour le contenu à droite

    // Ajouter des marges externes à vbox
    gtk_widget_set_margin_start(vbox, 20); // 20 pixels de marge à gauche
    gtk_widget_set_margin_top(vbox, 10);   // 10 pixels de marge en haut
    gtk_widget_set_margin_bottom(vbox, 10); // 10 pixels de marge en bas
    gtk_widget_set_margin_end(vbox, 80);    // 10 pixels de marge à droite

    // Créer une entrée pour le code-barres
    entry_code_barre = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry_code_barre, TRUE, FALSE, 5); // Prendre toute la largeur de la ligne

    // Ajouter un bouton pour scanner et ajouter du matériel
    button = gtk_button_new_with_label("Ajout Matériel");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_click), NULL);  
    gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, FALSE, 0); // Prendre toute la largeur de la ligne
    // Définir une taille minimale pour le bouton (largeur, hauteur)
    gtk_widget_set_size_request(button, 100, 50); // Ajustez la taille ici

    // Créer un label pour afficher le code-barres
    code_label = gtk_label_new("Aucun code-barres scanné.");

    // Créer un cadre (frame) pour contenir le label
    GtkWidget *frame = gtk_frame_new(NULL); // Aucun titre pour le cadre
    gtk_container_add(GTK_CONTAINER(frame), code_label); // Ajouter le label au cadre

    // Définir les bordures du cadre
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT); // Type d'ombre

    // Ajouter le cadre à la boîte verticale
    gtk_box_pack_start(GTK_BOX(vbox), frame, TRUE, FALSE, 0); // Prendre toute la largeur de la ligne

    // Créer un label pour afficher les messages
    message_label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox), message_label, TRUE, FALSE, 0); // Prendre toute la largeur de la ligne

    // --- Ajout de l'image centrée sous le champ de saisie ---
    GtkWidget *image = gtk_image_new_from_file("images/COD.png"); // Remplacez par le chemin de votre image

    // Créer une boîte horizontale pour centrer l'image
    GtkWidget *image_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(image_hbox), image, TRUE, TRUE, 0); // Ajouter l'image à la boîte

    // Ajouter la boîte d'image à la boîte verticale
    gtk_box_pack_start(GTK_BOX(vbox), image_hbox, TRUE, FALSE, 0);

    // Connexion pour fermer la fenêtre
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    return window;
}