#include "gestionPdf.h"    
#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>

void generate_pdf(GtkWidget *widget, gpointer data) {
    GtkListStore *store = GTK_LIST_STORE(data);
    GtkTreeIter iter;
    int id;
    const char *nom;

    // Créer un GtkFileChooserDialog pour permettre à l'utilisateur de choisir le nom du fichier
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new("Enregistrer le fichier PDF",
                                         NULL,
                                         GTK_FILE_CHOOSER_ACTION_SAVE,
                                         "Annuler", GTK_RESPONSE_CANCEL,
                                         "Enregistrer", GTK_RESPONSE_ACCEPT,
                                         NULL);

    // Suggérer un nom de fichier par défaut
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "inventaire.pdf");

    // Exécuter le dialogue et attendre que l'utilisateur choisisse un fichier ou annule
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        // Obtenir le chemin complet choisi par l'utilisateur
        char *pdf_path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

        printf("Le bouton PDF a été cliqué\n");  // Message de débogage

        // Créer une surface PDF avec le chemin spécifié par l'utilisateur
        cairo_surface_t *surface = cairo_pdf_surface_create(pdf_path, 595, 842);
        cairo_t *cr = cairo_create(surface);

        // Ajouter un titre au PDF
        cairo_set_font_size(cr, 20);
        
        // Déterminer la largeur du texte pour le centrer
        cairo_text_extents_t extents;
        const char *title = "Liste de l'inventaire 2024 - Mada-etech";
        cairo_text_extents(cr, title, &extents);
        
        // Calculer la position X pour centrer le titre
        double title_x = (595 - extents.width) / 2;
        cairo_move_to(cr, title_x, 50); // Position du titre (centré)

        // Afficher le titre
        cairo_show_text(cr, title);

        // Dessiner un tableau pour afficher les données
        cairo_set_font_size(cr, 12);
        int start_x = 50, start_y = 100;  // Position de départ du tableau
        int col_widths[] = {50, 300};     // Largeurs des colonnes (ID, Nom)
        int row_height = 20;              // Hauteur des lignes

        // En-tête du tableau
        cairo_rectangle(cr, start_x, start_y, col_widths[0], row_height);
        cairo_move_to(cr, start_x + 10, start_y + 15);  // Position texte ID
        cairo_show_text(cr, "ID");

        cairo_rectangle(cr, start_x + col_widths[0], start_y, col_widths[1], row_height);
        cairo_move_to(cr, start_x + col_widths[0] + 10, start_y + 15);  // Position texte Nom
        cairo_show_text(cr, "Nom");

        // Dessiner les lignes de l'en-tête
        cairo_stroke(cr);

        // Position de départ pour les lignes de données
        int current_y = start_y + row_height;

        // Récupérer les données de la liste et les insérer dans le tableau
        if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter)) {
            do {
                gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 0, &id, 1, &nom, -1); // Obtenir l'ID et le nom

                // Colonne ID
                cairo_rectangle(cr, start_x, current_y, col_widths[0], row_height);
                cairo_move_to(cr, start_x + 10, current_y + 15);
                char id_str[10];
                snprintf(id_str, sizeof(id_str), "%d", id);
                cairo_show_text(cr, id_str);

                // Colonne Nom
                cairo_rectangle(cr, start_x + col_widths[0], current_y, col_widths[1], row_height);
                cairo_move_to(cr, start_x + col_widths[0] + 10, current_y + 15);
                cairo_show_text(cr, nom);

                // Dessiner les lignes des données
                cairo_stroke(cr);

                // Passer à la ligne suivante
                current_y += row_height;
            } while (gtk_tree_model_iter_next(GTK_TREE_MODEL(store), &iter));
        }

        // Finaliser et fermer le PDF
        cairo_show_page(cr);
        cairo_destroy(cr);
        cairo_surface_destroy(surface);

        printf("PDF généré avec succès : %s\n", pdf_path);  // Confirmation

        // Ajouter un popup pour indiquer que le PDF a été généré avec succès
        GtkWidget *success_dialog;
        success_dialog = gtk_message_dialog_new(NULL,
                                                GTK_DIALOG_MODAL,
                                                GTK_MESSAGE_INFO,
                                                GTK_BUTTONS_OK,
                                                "Le PDF a été téléchargé avec succès sous le nom '%s'.", pdf_path);
        gtk_dialog_run(GTK_DIALOG(success_dialog));
        gtk_widget_destroy(success_dialog);

        // Libérer la mémoire du chemin
        g_free(pdf_path);
    }

    // Détruire la boîte de dialogue après l'avoir utilisée
    gtk_widget_destroy(dialog);
}
