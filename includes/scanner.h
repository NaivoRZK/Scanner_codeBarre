#ifndef SCANNER_H
#define SCANNER_H

#include <stddef.h> // Inclusion de la bibliothèque pour size_t

/**
 * @brief Lit un code-barres à partir de l'entrée standard.
 *
 * @param code_barre Pointeur vers la chaîne où le code-barres sera stocké.
 * @param size Taille maximale de la chaîne pour éviter le dépassement de tampon.
 * @return 1 en cas de succès, 0 en cas d'échec.
 */
int scan_code_barre(char *code_barre, size_t size);

#endif // SCANNER_H
