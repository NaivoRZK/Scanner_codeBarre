#include <stdio.h>
#include <string.h>
#include "scanner.h"

/**
 * @brief Lit un code-barres à partir de l'entrée standard.
 *
 * @param code_barre Pointeur vers la chaîne où le code-barres sera stocké.
 * @param size Taille maximale de la chaîne pour éviter le dépassement de tampon.
 * @return 1 en cas de succès, 0 en cas d'échec.
 */
int scan_code_barre(char *code_barre, size_t size) {
    // Lire une ligne de l'entrée standard et stocker dans code_barre
    if (fgets(code_barre, size, stdin) == NULL) {
        return 0; // Échec de la lecture
    }
    
    // Supprimer le caractère de nouvelle ligne '\n' si présent
    size_t len = strlen(code_barre);
    if (len > 0 && code_barre[len - 1] == '\n') {
        code_barre[len - 1] = '\0'; // Remplacer '\n' par '\0' pour terminer la chaîne
    }

    return 1; // Succès
}
