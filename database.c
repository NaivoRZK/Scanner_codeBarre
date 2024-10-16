#include <stdio.h>
#include <sqlite3.h>
#include "database.h"

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
        fprintf(stderr, "Impossible d'ouvrir la base de données: %s\n", sqlite3_errmsg(db));
        return; // Sortir si l'ouverture échoue
    }

    // Préparer la requête SQL pour insérer un nouvel article
    char sql[256];
    snprintf(sql, sizeof(sql), "INSERT INTO inventaire (nom) VALUES ('%s');", nom);

    // Exécuter la requête SQL
    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Erreur SQL: %s\n", err_msg); // Afficher le message d'erreur SQL
        sqlite3_free(err_msg); // Libérer la mémoire utilisée par le message d'erreur
    } else {
        printf("Article '%s' ajouté avec succès.\n", nom); // Confirmer l'ajout réussi
    }

    // Fermer la base de données
    sqlite3_close(db);
}
