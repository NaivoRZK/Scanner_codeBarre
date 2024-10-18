#include <stdio.h>              // Inclusion de la bibliothèque standard pour les entrées/sorties
#include <sqlite3.h>           // Inclusion de la bibliothèque SQLite pour la gestion des bases de données
#include "database.h"          // Inclusion du fichier d'en-tête personnalisé pour les déclarations de fonctions
#include <gtk/gtk.h>           // Inclusion de la bibliothèque GTK pour l'interface graphique
#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h> 
#include "gestionPdf.h"   


/**
 * @brief Ajoute un article dans la base de données.
 *
 * @param db_name Le nom de la base de données.
 * @param nom Le nom de l'article à ajouter.
 */
void add_item(const char *db_name, const char *nom) {
    sqlite3 *db;             // Pointeur vers la base de données
    char *err_msg = 0;      // Pointeur pour stocker les messages d'erreur

    // Ouvrir la base de données
    if (sqlite3_open(db_name, &db) != SQLITE_OK) {
        // Afficher une erreur si l'ouverture échoue
        fprintf(stderr, "Impossible d'ouvrir la base de données: %s\n", sqlite3_errmsg(db));
        return; // Sortir si l'ouverture échoue
    }

    // Préparer la requête SQL pour insérer un nouvel article
    char sql[256];
    snprintf(sql, sizeof(sql), "INSERT INTO inventaire (nom) VALUES ('%s');", nom); // Formater la requête SQL

    // Exécuter la requête SQL
    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
        // Afficher le message d'erreur SQL si la requête échoue
        fprintf(stderr, "Erreur SQL: %s\n", err_msg);
        sqlite3_free(err_msg); // Libérer la mémoire utilisée par le message d'erreur
    } else {
        // Confirmer l'ajout réussi
        printf("Article '%s' ajouté avec succès.\n", nom);
    }

    // Fermer la base de données
    sqlite3_close(db);
}

// Fonction de gestion de la suppression
void on_delete_button_clicked(GtkWidget *widget, gpointer data) {
    GtkTreeSelection *selection = (GtkTreeSelection *)data; // Récupérer la sélection dans le TreeView
    GtkTreeModel *model;      // Modèle de données pour le TreeView
    GtkTreeIter iter;         // Itérateur pour parcourir les éléments
    gint id;                  // ID de l'élément à supprimer

    // Obtenir l'élément sélectionné
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        // Récupérer l'ID de l'élément sélectionné
        gtk_tree_model_get(model, &iter, 0, &id, -1);

        // Boîte de dialogue de confirmation
        GtkWidget *dialog;
        dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                        GTK_DIALOG_MODAL,              // Dialogue modal
                                        GTK_MESSAGE_QUESTION,          // Type de message
                                        GTK_BUTTONS_YES_NO,            // Boutons de réponse
                                        "Êtes-vous sûr de vouloir supprimer l'élément avec ID %d ?", id);
        
        // Afficher la boîte de dialogue
        gint response = gtk_dialog_run(GTK_DIALOG(dialog)); // Attendre la réponse de l'utilisateur

        if (response == GTK_RESPONSE_YES) { // Si l'utilisateur confirme la suppression
            sqlite3 *db;                   // Pointeur vers la base de données
            char *err_msg = 0;            // Pointeur pour les messages d'erreur

            // Ouvrir la base de données
            if (sqlite3_open("db/inventaire.db", &db) == SQLITE_OK) {
                char sql[256];             // Tableau pour la requête SQL
                sprintf(sql, "DELETE FROM inventaire WHERE id = %d;", id); // Créer la requête de suppression

                // Exécuter la requête SQL
                if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
                    // Afficher l'erreur si la suppression échoue
                    fprintf(stderr, "Erreur lors de la suppression: %s\n", err_msg);
                    sqlite3_free(err_msg); // Libérer la mémoire utilisée par le message d'erreur
                } else {
                    // Afficher un message de succès
                    GtkWidget *success_dialog;
                    success_dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                            GTK_DIALOG_MODAL,
                                                            GTK_MESSAGE_INFO,
                                                            GTK_BUTTONS_OK,
                                                            "L'élément avec ID %d a été supprimé avec succès.", id);
                    gtk_dialog_run(GTK_DIALOG(success_dialog)); // Afficher le message de succès
                    gtk_widget_destroy(success_dialog); // Détruire la boîte de dialogue après utilisation
                }

                // Réorganiser les IDs (facultatif)
                sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL); // Démarrer une transaction
                sprintf(sql, "UPDATE inventaire SET id = rowid WHERE rowid > (SELECT MIN(rowid) FROM inventaire WHERE id = %d);", id);
                sqlite3_exec(db, sql, NULL, NULL, NULL); // Réorganiser les IDs
                sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL); // Valider la transaction

                sqlite3_close(db); // Fermer la base de données
            }
        }

        // Fermer la boîte de dialogue de confirmation
        gtk_widget_destroy(dialog);

        // Optionnel: Actualiser la vue ou fermer la boîte de dialogue après suppression
        gtk_widget_destroy(gtk_widget_get_toplevel(widget)); // Ferme la boîte de dialogue
        show_all_items(); // Rappel pour montrer les items mis à jour
    }
}



// Fonction pour afficher tous les items
void show_all_items() {
    sqlite3 *db;                   // Pointeur vers la base de données
    sqlite3_stmt *stmt;           // Pointeur vers la déclaration SQL
    GtkWidget *dialog;            // Boîte de dialogue pour afficher les items
    GtkWidget *content_area;      // Zone de contenu de la boîte de dialogue
    GtkWidget *treeview;          // Widget TreeView pour afficher les données
    GtkListStore *store;          // Store pour contenir les données
    GtkTreeIter iter;             // Itérateur pour le store
    GtkWidget *delete_button;     // Bouton de suppression
    GtkWidget *pdf_button;        // Bouton pour générer le PDF
    GtkTreeSelection *selection;  // Sélection pour le TreeView

    // Ouvrir la base de données
    if (sqlite3_open("db/inventaire.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Impossible d'ouvrir la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Préparer la requête SQL
    const char *sql = "SELECT id, nom FROM inventaire;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    // Création de la boîte de dialogue
    dialog = gtk_dialog_new_with_buttons("Tous les Matériels", NULL, GTK_DIALOG_MODAL, "OK", GTK_RESPONSE_OK, NULL);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 600, 400);

    // Créer un GtkListStore pour le TreeView
    store = gtk_list_store_new(2, G_TYPE_INT, G_TYPE_STRING);

    // Remplir le store avec les données de la base de données
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *nom = (const char *)sqlite3_column_text(stmt, 1);
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, id, 1, nom, -1);
    }

    // Libérer les ressources
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // Créer le TreeView
    treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    
    // Ajouter les colonnes au TreeView
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, "ID", renderer, "text", 0, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, "Nom", renderer, "text", 1, NULL);

    // Appliquer un style pour ajouter une bordure
    gtk_widget_set_margin_start(treeview, 10);
    gtk_widget_set_margin_end(treeview, 10);
    gtk_widget_set_margin_top(treeview, 10);
    gtk_widget_set_margin_bottom(treeview, 10);

    // Ajouter le TreeView au contenu de la boîte de dialogue
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_box_pack_start(GTK_BOX(content_area), treeview, TRUE, TRUE, 0);

    // Ajouter un bouton de suppression
    delete_button = gtk_button_new_with_label("Supprimer");
    gtk_box_pack_start(GTK_BOX(content_area), delete_button, FALSE, FALSE, 5);

    // Ajouter un bouton pour générer le PDF
    pdf_button = gtk_button_new_with_label("Télécharger PDF");
    gtk_box_pack_start(GTK_BOX(content_area), pdf_button, FALSE, FALSE, 5); // Ajouter le bouton PDF

    // Sélection de l'élément
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

    // Connexion pour le bouton de suppression
    g_signal_connect(delete_button, "clicked", G_CALLBACK(on_delete_button_clicked), selection);

    // Connexion pour le bouton de téléchargement PDF
    g_signal_connect(pdf_button, "clicked", G_CALLBACK(generate_pdf), store); // Passer le store ici
    // Afficher la boîte de dialogue
    gtk_widget_show_all(dialog);

    // Connexion pour fermer la boîte de dialogue
    g_signal_connect(dialog, "response", G_CALLBACK(gtk_widget_destroy), NULL);
}